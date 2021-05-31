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
import shutil
import tempfile
from urllib.request import urlopen
from nnunet.paths import network_training_output_dir
from subprocess import call
import requests


def get_available_models():
    available_models = {
        "Task001_BrainTumour": {
            'description': "Brain Tumor Segmentation. \n"
                           "Segmentation targets are edema, enhancing tumor and necrosis, \n"
                           "input modalities are 0: FLAIR, 1: T1, 2: T1 with contrast agent, 3: T2. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task001_BrainTumour.zip?download=1"
        },
        "Task002_Heart": {
            'description': "Left Atrium Segmentation. \n"
                           "Segmentation target is the left atrium, \n"
                           "input modalities are 0: MRI. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task002_Heart.zip?download=1"
        },
        "Task003_Liver": {
            'description': "Liver and Liver Tumor Segmentation. \n"
                           "Segmentation targets are liver and tumors, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task003_Liver.zip?download=1"
        },
        "Task004_Hippocampus": {
            'description': "Hippocampus Segmentation. \n"
                           "Segmentation targets posterior and anterior parts of the hippocampus, \n"
                           "input modalities are 0: MRI. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task004_Hippocampus.zip?download=1"
        },
        "Task005_Prostate": {
            'description': "Prostate Segmentation. \n"
                           "Segmentation targets are peripheral and central zone, \n"
                           "input modalities are 0: T2, 1: ADC. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task005_Prostate.zip?download=1"
        },
        "Task006_Lung": {
            'description': "Lung Nodule Segmentation. \n"
                           "Segmentation target are lung nodules, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task006_Lung.zip?download=1"
        },
        "Task007_Pancreas": {
            'description': "Pancreas Segmentation. \n"
                           "Segmentation targets are pancras and pancreas tumor, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task007_Pancreas.zip?download=1"
        },
        "Task008_HepaticVessel": {
            'description': "Hepatic Vessel Segmentation. \n"
                           "Segmentation targets are hepatic vesels and liver tumors, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task008_HepaticVessel.zip?download=1"
        },
        "Task009_Spleen": {
            'description': "Spleen Segmentation. \n"
                           "Segmentation target is the spleen, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task009_Spleen.zip?download=1"
        },
        "Task010_Colon": {
            'description': "Colon Cancer Segmentation. \n"
                           "Segmentation target are colon caner primaries, \n"
                           "input modalities are 0: CT scan. \n"
                           "Also see Medical Segmentation Decathlon, http://medicaldecathlon.com/",
            'url': "https://zenodo.org/record/3734294/files/Task010_Colon.zip?download=1"
        },
        "Task017_AbdominalOrganSegmentation": {
            'description': "Multi-Atlas Labeling Beyond the Cranial Vault - Abdomen. \n"
                           "Segmentation targets are thirteen different abdominal organs, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see https://www.synapse.org/#!Synapse:syn3193805/wiki/217754",
            'url': "https://zenodo.org/record/3734294/files/Task017_AbdominalOrganSegmentation.zip?download=1"
        },
        "Task024_Promise": {
            'description': "Prostate MR Image Segmentation 2012. \n"
                           "Segmentation target is the prostate, \n"
                           "input modalities are 0: T2. \n"
                           "Also see https://promise12.grand-challenge.org/",
            'url': "https://zenodo.org/record/3734294/files/Task024_Promise.zip?download=1"
        },
        "Task027_ACDC": {
            'description': "Automatic Cardiac Diagnosis Challenge. \n"
                           "Segmentation targets are right ventricle, left ventricular cavity and left myocardium, \n"
                           "input modalities are 0: cine MRI. \n"
                           "Also see https://acdc.creatis.insa-lyon.fr/",
            'url': "https://zenodo.org/record/3734294/files/Task027_ACDC.zip?download=1"
        },
        "Task029_LiTS": {
            'description': "Liver and Liver Tumor Segmentation Challenge. \n"
                           "Segmentation targets are liver and liver tumors, \n"
                           "input modalities are 0: abdominal CT scan. \n"
                           "Also see https://competitions.codalab.org/competitions/17094",
            'url': "https://zenodo.org/record/3734294/files/Task029_LITS.zip?download=1"
        },
        "Task035_ISBILesionSegmentation": {
            'description': "Longitudinal multiple sclerosis lesion segmentation Challenge. \n"
                           "Segmentation target is MS lesions, \n"
                           "input modalities are 0: FLAIR, 1: MPRAGE, 2: proton density, 3: T2. \n"
                           "Also see https://smart-stats-tools.org/lesion-challenge",
            'url': "https://zenodo.org/record/3734294/files/Task035_ISBILesionSegmentation.zip?download=1"
        },
        "Task038_CHAOS_Task_3_5_Variant2": {
            'description': "CHAOS - Combined (CT-MR) Healthy Abdominal Organ Segmentation Challenge (Task 3 & 5). \n"
                           "Segmentation targets are left and right kidney, liver, spleen, \n"
                           "input modalities are 0: T1 in-phase, T1 out-phase, T2 (can be any of those)\n"
                           "Also see https://chaos.grand-challenge.org/",
            'url': "https://zenodo.org/record/3734294/files/Task038_CHAOS_Task_3_5_Variant2.zip?download=1"
        },
        "Task048_KiTS_clean": {
            'description': "Kidney and Kidney Tumor Segmentation Challenge. "
                           "Segmentation targets kidney and kidney tumors, "
                           "input modalities are 0: abdominal CT scan. "
                           "Also see https://kits19.grand-challenge.org/",
            'url': "https://zenodo.org/record/3734294/files/Task048_KiTS_clean.zip?download=1"
        },
        "Task055_SegTHOR": {
            'description': "SegTHOR: Segmentation of THoracic Organs at Risk in CT images. \n"
                           "Segmentation targets are aorta, esophagus, heart and trachea, \n"
                           "input modalities are 0: CT scan. \n"
                           "Also see https://competitions.codalab.org/competitions/21145",
            'url': "https://zenodo.org/record/3734294/files/Task055_SegTHOR.zip?download=1"
        },
        "Task061_CREMI": {
            'description': "MICCAI Challenge on Circuit Reconstruction from Electron Microscopy Images (Synaptic Cleft segmentation task). \n"
                           "Segmentation target is synaptic clefts, \n"
                           "input modalities are 0: serial section transmission electron microscopy of neural tissue. \n"
                           "Also see https://cremi.org/",
            'url': "https://zenodo.org/record/3734294/files/Task061_CREMI.zip?download=1"
        },

    }
    return available_models


def print_available_pretrained_models():
    print('The following pretrained models are available:\n')
    av_models = get_available_models()
    for m in av_models.keys():
        print('')
        print(m)
        print(av_models[m]['description'])


def download_and_install_pretrained_model_by_name(taskname):
    av_models = get_available_models()
    if taskname not in av_models.keys():
        raise RuntimeError("\nThe requested pretrained model ('%s') is not available." % taskname)
    if len(av_models[taskname]['url']) == 0:
        raise RuntimeError("The requested model has not been uploaded yet. Please check back in a few days")
    download_and_install_from_url(av_models[taskname]['url'])


def download_and_install_from_url(url):
    assert network_training_output_dir is not None, "Cannot install model because network_training_output_dir is not " \
                                                    "set (RESULTS_FOLDER missing as environment variable, see " \
                                                    "Installation instructions)"
    import http.client
    http.client.HTTPConnection._http_vsn = 10
    http.client.HTTPConnection._http_vsn_str = 'HTTP/1.0'

    with tempfile.NamedTemporaryFile() as f:
        fname = f.name
        print("Downloading pretrained model", url)
        data = urlopen(url).read()
        f.write(data)
        # unzip -o zip_file -d output_dir
        print("Download finished. Extracting...")
        install_model_from_zip_file(fname)
        print("Done")


def download_and_install_from_url2(url):
    assert network_training_output_dir is not None, "Cannot install model because network_training_output_dir is not " \
                                                    "set (RESULTS_FOLDER missing as environment variable, see " \
                                                    "Installation instructions)"
    import http.client
    http.client.HTTPConnection._http_vsn = 10
    http.client.HTTPConnection._http_vsn_str = 'HTTP/1.0'

    with tempfile.NamedTemporaryFile() as f:
        fname = f.name
        print("Downloading pretrained model", url)
        r = requests.get(url)
        f.write(r.content)

        print("Download finished. Extracting...")
        install_model_from_zip_file(fname)
        print("Done")


def install_model_from_zip_file(zip_file: str):
    call(['unzip', '-o', '-d', network_training_output_dir, zip_file])


def print_license_warning():
    print('')
    print('######################################################')
    print('!!!!!!!!!!!!!!!!!!!!!!!!WARNING!!!!!!!!!!!!!!!!!!!!!!!')
    print('######################################################')
    print("Using the pretrained model weights is subject to the license of the dataset they were trained on. Some "
          "allow commercial use, others don't. It is your responsibility to make sure you use them appropriately! Use "
          "nnUNet_print_pretrained_model_info(task_name) to see a summary of the dataset and where to find its license!")
    print('######################################################')
    print('')


def download_by_name():
    import argparse
    parser = argparse.ArgumentParser(description="Use this to download pretrained models. CAREFUL: This script will "
                                                 "overwrite "
                                                 "existing models (if they share the same trainer class and plans as "
                                                 "the pretrained model")
    parser.add_argument("task_name", type=str, help='Task name of the pretrained model. To see '
                                                                   'available task names, run nnUNet_print_available_'
                                                                   'pretrained_models')
    args = parser.parse_args()
    taskname = args.task_name
    print_license_warning()
    download_and_install_pretrained_model_by_name(taskname)


def download_by_url():
    import argparse
    parser = argparse.ArgumentParser(
        description="Use this to download pretrained models. This script is intended to download models via url only. "
                    "If you want to download one of our pretrained models, please use nnUNet_download_pretrained_model. "
                    "CAREFUL: This script will overwrite "
                    "existing models (if they share the same trainer class and plans as "
                    "the pretrained model.")
    parser.add_argument("url", type=str, help='URL of the pretrained model')
    args = parser.parse_args()
    url = args.url
    download_and_install_from_url(url)


def install_from_zip_entry_point():
    import argparse
    parser = argparse.ArgumentParser(
        description="Use this to install a zip file containing a pretrained model.")
    parser.add_argument("zip", type=str, help='zip file')
    args = parser.parse_args()
    zip = args.zip
    install_model_from_zip_file(zip)


def print_pretrained_model_requirements():
    import argparse
    parser = argparse.ArgumentParser(description="Use this to see the properties of a pretrained model, especially "
                                                 "what input modalities it requires")
    parser.add_argument("task_name", type=str, help='Task name of the pretrained model. To see '
                                                                   'available task names, run nnUNet_print_available_'
                                                                   'pretrained_models')
    args = parser.parse_args()
    taskname = args.task_name
    av = get_available_models()
    if taskname not in av.keys():
        raise RuntimeError("Invalid task name. This pretrained model does not exist. To see available task names, "
                           "run nnUNet_print_available_pretrained_models")
    print(av[taskname]['description'])


if __name__  == '__main__':
    url = 'https://www.dropbox.com/s/ft54q1gi060vm2x/Task004_Hippocampus.zip?dl=1'