
# Semi-supervised Learning Baseline

## Introduction

Our baseline method is derived from Noisy-Student learning method in semi-supervised image classification [1] and semi-supervised urban scene segmentation [2] tasks. We employ 3D nnU-Net [3] for both teacher and student models. It includes five main steps:

* Step 1. Training a teacher model on the manually labelled data.
* Step 2. Generating pseudo labels of the unlabelled data via the teacher model.
* Step 3. Training a student model on both manually and pseudo-labelled data.
* Step 4. Finetuning the student model in step 3 on the manually labelled data.
* Step 5. Going back to step 2 and replacing the teacher model with the student model for a desired number of iterations.

## Training

Training from scratch:

`nnUNet_train 3d_fullres nnUNetTrainerV2 TaskXXX_MYTASK all`

Finetuing:

`nnUNet_train 3d_fullres nnUNetTrainerV2Finetune TaskXXX_MYTASK all`

## Trained Models

- [Dropbox](https://www.dropbox.com/sh/1wydqhoan040rdp/AADES1dHc0S1lqvGiDLWQfmKa?dl=0)
- [Baidu Net Disk](https://pan.baidu.com/s/1vwn88HBGwsAOpcy0Q2qXyQ ) (PW:2021)

## Inference

Run

`nnUNet_predict -i INPUT_FOLDER -o OUTPUT_FOLDER -t TASK_NAME_OR_ID -m 2d -tr nnUNetTrainerV2Finetune`

## Reference

[1] Q. Xie, M.-T. Luong, E. Hovy, and Q. V. Le, “Self-training with noisy student improves imagenet classification,” in Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition, 2020, pp. 10 687–10 698.

[2] L.-C. Chen, R. G. Lopes, B. Cheng, M. D. Collins, E. D. Cubuk, B. Zoph, H. Adam, and J. Shlens, “Semi-supervised learning in video sequences for urban scene segmentation,” European Conference on Computer Vision, 2020.

[3] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, “nnu-net: a self-configuring method for deep learning-based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021.



## Acknowledgements

We highly appreciate the out-of-the-box implementation of [nnU-Net](https://github.com/MIC-DKFZ/nnUNet) (by [Dr. Fabian Isensee](https://github.com/FabianIsensee)).
