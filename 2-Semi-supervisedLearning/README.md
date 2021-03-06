
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

```
nnUNet_train 3d_fullres nnUNetTrainerV2 TaskXXX_MYTASK all

```
Finetuing:

```

nnUNet_train 3d_fullres nnUNetTrainerV2Finetune TaskXXX_MYTASK all

```

## Trained Models

- [Dropbox](https://www.dropbox.com/sh/r2x84oz3ycyhcvn/AAAMOEZUQdFRuDqGb46ok3Qca?dl=0)
- [Baidu Net Disk](https://pan.baidu.com/s/1-kTRtXrtCppuRCN-QTgAVA) (PW:2021)

## Inference

Run
```
nnUNet_predict -i INPUT_FOLDER -o OUTPUT_FOLDER -t TASK_NAME_OR_ID -m 3d_fullres -tr nnUNetTrainerV2Finetune -f all --disable_tta
```


## Baseline Docker

- [Subtask 1: 41 labelled cases + 400 unlabelled cases](https://hub.docker.com/repository/docker/gospelslave/semi-suplearn_subtask1)

Run

```shell
# subtask1
# pull docker image from hub.docker.com
docker pull gospelslave/semi-suplearn_subtask1

# docker predict command
docker container run --gpus "device=1" --name semi-suplearn_subtask1 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/semi-suplearn_subtask1_outputs/:/workspace/outputs/ gospelslave/semi-suplearn_subtask1:latest /bin/bash -c "sh predict.sh"

# subtask2
# pull docker image from hub.docker.com
docker pull gospelslave/semi-suplearn_subtask2

# docker predict command
docker container run --gpus "device=1" --name semi-suplearn_subtask2 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/semi-suplearn_subtask2_outputs/:/workspace/outputs/ gospelslave/semi-suplearn_subtask2:latest /bin/bash -c "sh predict.sh"

# lowerbound
# pull docker image from hub.docker.com
docker pull gospelslave/semi-suplearn_lowerbound

# docker predict command
docker container run --gpus "device=1" --name semi-suplearn_lowerbound --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/semi-suplearn_lowerbound_outputs/:/workspace/outputs/ gospelslave/semi-suplearn_lowerbound:latest /bin/bash -c "sh predict.sh"

# upperbound
# pull docker image from hub.docker.com
docker pull gospelslave/semi-suplearn_upperbound

# docker predict command
docker container run --gpus "device=1" --name semi-suplearn_upperbound --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/semi-suplearn_upperbound_outputs/:/workspace/outputs/ gospelslave/semi-suplearn_upperbound:latest /bin/bash -c "sh predict.sh"

```



- Subtask 2: 41 labelled cases + 800 unlabelled cases





## Reference

[1] Q. Xie, M.-T. Luong, E. Hovy, and Q. V. Le, “Self-training with noisy student improves imagenet classification,” in Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition, 2020, pp. 10 687–10 698.

[2] L.-C. Chen, R. G. Lopes, B. Cheng, M. D. Collins, E. D. Cubuk, B. Zoph, H. Adam, and J. Shlens, “Semi-supervised learning in video sequences for urban scene segmentation,” European Conference on Computer Vision, 2020.

[3] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, “nnu-net: a self-configuring method for deep learning-based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021.



## Acknowledgements

We highly appreciate the out-of-the-box implementation of [nnU-Net](https://github.com/MIC-DKFZ/nnUNet) (by [Dr. Fabian Isensee](https://github.com/FabianIsensee)).
