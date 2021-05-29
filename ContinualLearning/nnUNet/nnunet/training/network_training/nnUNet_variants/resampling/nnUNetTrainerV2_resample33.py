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


from nnunet.inference.segmentation_export import save_segmentation_nifti_from_softmax
from nnunet.training.network_training.nnUNetTrainerV2 import nnUNetTrainerV2


class nnUNetTrainerV2_resample33(nnUNetTrainerV2):
    def validate(self, do_mirroring: bool = True, use_sliding_window: bool = True, step_size: float = 0.5,
                 save_softmax: bool = True, use_gaussian: bool = True, overwrite: bool = True,
                 validation_folder_name: str = 'validation_raw', debug: bool = False, all_in_gpu: bool = False,
                 force_separate_z: bool = None, interpolation_order: int = 3, interpolation_order_z=0):
        return super().validate(do_mirroring, use_sliding_window, step_size, save_softmax, use_gaussian,
                                overwrite, validation_folder_name, debug, all_in_gpu,
                                force_separate_z=False, interpolation_order=3,
                                interpolation_order_z=3)

    def preprocess_predict_nifti(self, input_files, output_file=None, softmax_ouput_file=None):
        """
        Use this to predict new data
        :param input_files:
        :param output_file:
        :param softmax_ouput_file:
        :return:
        """
        print("preprocessing...")
        d, s, properties = self.preprocess_patient(input_files)
        print("predicting...")
        pred = self.predict_preprocessed_data_return_seg_and_softmax(d, self.data_aug_params["do_mirror"],
                                                                     self.data_aug_params['mirror_axes'], True, 0.5,
                                                                     True, 'constant', {'constant_values': 0},
                                                                     self.patch_size, True)[1]
        pred = pred.transpose([0] + [i + 1 for i in self.transpose_backward])

        print("resampling to original spacing and nifti export...")
        save_segmentation_nifti_from_softmax(pred, output_file, properties, 3, None, None, None, softmax_ouput_file,
                                             None, force_separate_z=False, interpolation_order_z=3)
        print("done")
