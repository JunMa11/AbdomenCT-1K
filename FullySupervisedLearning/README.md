# Fully Supervised Baseline

## Introduction

The baseline is built on the default `3d_fullres` model in 2D nnUNet [1].

## Training

Run

`nnUNet_train 3d_fullres nnUNetTrainerV2 TaskXXX_MYTASK all`

## Trained Models

- [Dropbox](https://www.dropbox.com/s/jy9jmnwypumtm9h/BaselineModel.zip?dl=0)
- [Baidu Net Disk](https://pan.baidu.com/s/1vwn88HBGwsAOpcy0Q2qXyQ ) (PW:2021)

## Inference

Run

`nnUNet_predict -i INPUT_FOLDER -o OUTPUT_FOLDER -t TASK_NAME_OR_ID -m 3d_fullres --disable_tta -f all`



## Baseline Docker

- [Subtask 1](https://hub.docker.com/repository/docker/gospelslave/fully_suplearn_subtask1)

Run

`docker pull gospelslave/fully_suplearn_subtask1`

`docker container run --gpus "device=1" --name fully_suplearn_subtask1 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/fully_suplearn_subtask1_outputs/:/workspace/outputs/ gospelslave/fully_suplearn_subtask1:latest /bin/bash -c "sh predict.sh"`

- Subtask 2

## Reference

[1] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, "nnU-Net: a self-confifiguring method for deep learning" based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021. 

