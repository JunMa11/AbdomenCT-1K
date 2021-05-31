#    Copyright 2020 Division of Medical Image Computing, German Cancer Research Center (DKFZ), Heidelberg, Germany
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.


from itertools import combinations
import nnunet
from batchgenerators.utilities.file_and_folder_operations import *
from nnunet.paths import network_training_output_dir
import numpy as np
from subprocess import call
from nnunet.postprocessing.consolidate_postprocessing import consolidate_folds
from nnunet.utilities.folder_names import get_output_folder_name
from nnunet.paths import default_cascade_trainer, default_trainer, default_plans_identifier


def find_task_name(folder, task_id):
    candidates = subdirs(folder, prefix="Task%03.0d_" % task_id, join=False)
    assert len(candidates) > 0, "no candidate for Task id %d found in folder %s" % (task_id, folder)
    assert len(candidates) == 1, "more than one candidate for Task id %d found in folder %s" % (task_id, folder)
    return candidates[0]


def get_mean_foreground_dice(json_file):
    results = load_json(json_file)
    results_mean = results['results']['mean']
    dice_scores = [results_mean[i]['Dice'] for i in results_mean.keys() if i != "0"]
    return np.mean(dice_scores)


def main():
    import argparse
    parser = argparse.ArgumentParser(usage="This is intended to identify the best model based on the five fold "
                                           "cross-validation. Running this script requires all models to have been run "
                                           "already. This script will summarize the results of the five folds of all "
                                           "models in one json each for easy interpretability")

    parser.add_argument("-m", '--models', nargs="+", required=False, default=['2d', '3d_lowres', '3d_fullres',
                                                                              '3d_cascade_fullres'])
    parser.add_argument("-t", '--task_ids', nargs="+", required=True)

    parser.add_argument("-tr", type=str, required=False, default=default_trainer,
                           help="nnUNetTrainer class. Default: %s" % default_trainer)
    parser.add_argument("-ctr", type=str, required=False, default=default_cascade_trainer,
                           help="nnUNetTrainer class for cascade model. Default: %s" % default_cascade_trainer)
    parser.add_argument("-pl", type=str, required=False, default=default_plans_identifier,
                           help="plans name, Default: %s" % default_plans_identifier)

    parser.add_argument("--strict", required=False, default=False, action="store_true",
                        help="set this flag if you want this script to crash of one of the models is missing")

    args = parser.parse_args()
    tasks = [int(i) for i in args.task_ids]

    models = args.models
    tr = args.tr
    trc = args.ctr
    strict = args.strict
    pl = args.pl

    validation_folder = "validation_raw"

    # this script now acts independently from the summary jsons. That was unnecessary
    id_task_mapping = {}
    # for each task, run ensembling using all combinations of two models
    for t in tasks:
        # first collect pure model performance (postprocessed)
        results = {}
        all_results = {}
        valid_models = []
        for m in models:
            try:
                if m == "3d_cascade_fullres":
                    trainer = trc
                else:
                    trainer = tr

                if t not in id_task_mapping.keys():
                    task_name = find_task_name(get_output_folder_name(m), t)
                    id_task_mapping[t] = task_name

                output_folder = get_output_folder_name(m, id_task_mapping[t], trainer, pl)
                assert isdir(output_folder), "Output folder for model %s is missing, expected: %s" % (m, output_folder)

                # we need a postprocessing_json for inference, so that must be present
                postprocessing_json = join(output_folder, "postprocessing.json")
                # we need cv_niftis_postprocessed to know the single model performance
                cv_niftis_folder = join(output_folder, "cv_niftis_raw")
                if not isfile(postprocessing_json) or not isdir(cv_niftis_folder):
                    print("running missing postprocessing for %s and model %s" % (id_task_mapping[t], m))
                    consolidate_folds(output_folder)
                assert isfile(postprocessing_json), "Postprocessing json missing, expected: %s" % postprocessing_json
                assert isdir(cv_niftis_folder), "Folder with niftis from CV missing, expected: %s" % cv_niftis_folder

                # obtain mean foreground dice
                summary_file = join(cv_niftis_folder, "summary.json")
                results[m] = get_mean_foreground_dice(summary_file)
                all_results[m] = load_json(summary_file)['results']['mean']
                valid_models.append(m)

            except Exception as e:
                if strict:
                    raise e
                else:
                    print("WARNING!")
                    print(e)

        # now run ensembling and add ensembling to results
        print("\nFound the following valid models:\n", valid_models)
        if len(valid_models) > 1:
            for m1, m2 in combinations(valid_models, 2):

                trainer_m1 = trc if m1 == "3d_cascade_fullres" else tr
                trainer_m2 = trc if m2 == "3d_cascade_fullres" else tr

                ensemble_name = "ensemble_" + m1 + "__" + trainer_m1 + "__" + pl + "--" + m2 + "__" + trainer_m2 + "__" + pl
                output_folder_base = join(network_training_output_dir, "ensembles", id_task_mapping[t], ensemble_name)

                network1_folder = get_output_folder_name(m1, id_task_mapping[t], trainer_m1, pl)
                network2_folder = get_output_folder_name(m2, id_task_mapping[t], trainer_m2, pl)

                print("ensembling", network1_folder, network2_folder)
                p = call(["python", join(nnunet.__path__[0], "evaluation/model_selection/ensemble.py"),
                          network1_folder, network2_folder, output_folder_base, id_task_mapping[t], validation_folder])
                if p != 0:
                    raise RuntimeError("ensembling failed, see error message above (most likely you did not run model validation with --npz)")
                # ensembling will automatically do postprocessing

                # now get result of ensemble
                results[ensemble_name] = get_mean_foreground_dice(join(output_folder_base, "ensembled_raw", "summary.json"))
                all_results[ensemble_name] = load_json(join(output_folder_base, "ensembled_raw", "summary.json"))['results']['mean']

        # now print all mean foreground dice and highlight the best
        foreground_dices = list(results.values())
        best = np.max(foreground_dices)
        for k, v in results.items():
            print(k, v)

        predict_str = ""
        best_model = None
        for k, v in results.items():
            if v == best:
                print("%s submit model %s" % (id_task_mapping[t], k), v)
                best_model = k
                print("\nHere is how you should predict test cases. Run in sequential order and replace all input and output folder names with your personalized ones\n")
                if k.startswith("ensemble"):
                    tmp = k[len("ensemble_"):]
                    model1, model2 = tmp.split("--")
                    m1, t1, pl1 = model1.split("__")
                    m2, t2, pl2 = model2.split("__")
                    predict_str += "nnUNet_predict -i FOLDER_WITH_TEST_CASES -o OUTPUT_FOLDER_MODEL1 -tr " + tr + " -ctr " + trc + " -m " + m1 + " -p " + pl + " -t " + \
                                   id_task_mapping[t] + "\n"
                    predict_str += "nnUNet_predict -i FOLDER_WITH_TEST_CASES -o OUTPUT_FOLDER_MODEL2 -tr " + tr + " -ctr " + trc + " -m " + m2 + " -p " + pl + " -t " + \
                                   id_task_mapping[t] + "\n"

                    predict_str += "nnUNet_ensemble -f OUTPUT_FOLDER_MODEL1 OUTPUT_FOLDER_MODEL2 -o OUTPUT_FOLDER -pp " + join(network_training_output_dir, "ensembles", id_task_mapping[t], k, "postprocessing.json") + "\n"
                else:
                    predict_str += "nnUNet_predict -i FOLDER_WITH_TEST_CASES -o OUTPUT_FOLDER_MODEL1 -tr " + tr + " -ctr " + trc + " -m " + k + " -p " + pl + " -t " + \
                                   id_task_mapping[t] + "\n"
                print(predict_str)

        summary_folder = join(network_training_output_dir, "ensembles", id_task_mapping[t])
        with open(join(summary_folder, "prediction_commands.txt"), 'w') as f:
            f.write(predict_str)

        num_classes = len(all_results[best_model].keys())
        with open(join(summary_folder, "summary.csv"), 'w') as f:
            f.write("model")
            for c in range(1, num_classes + 1):
                f.write(",class%d" % c)
            f.write(",average")
            f.write("\n")
            for m in all_results.keys():
                f.write(m)
                for c in range(1, num_classes + 1):
                    f.write(",%01.4f" % all_results[m][str(c)]["Dice"])
                f.write(",%01.4f" % results[m])
                f.write("\n")


if __name__ == "__main__":
    main()
