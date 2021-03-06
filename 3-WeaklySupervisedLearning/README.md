# Weakly Supervised Learning Baseline

## Introduction

The baseline is built on the combination of 2D nnUNet [1] and fully connected Conditional Random Fields (CRF)[2], which is motivated by Gao et al.'s method in addressing the missing annotation problem [3].



## Training

Run

`nnUNet_train 2d nnUNetTrainerV2 TaskXXX_MYTASK all`

## Trained Models

- [Dropbox](https://www.dropbox.com/s/jy9jmnwypumtm9h/BaselineModel.zip?dl=0)
- [Baidu Net Disk](https://pan.baidu.com/s/1vwn88HBGwsAOpcy0Q2qXyQ ) (PW:2021)

## Inference

Run

`nnUNet_predict -i INPUT_FOLDER -o OUTPUT_FOLDER -t TASK_NAME_OR_ID -m 2d -disable_tta -f all --save_npz `


## Baseline Docker

- [Subtask 1:  5% labels](https://hub.docker.com/repository/docker/gospelslave/weakly_suplearn_subtask1)

Run

```

# pull docker image from hub.docker.com
docker pull gospelslave/weakly_suplearn_subtask1

# docker predict command
docker container run --gpus "device=1" --name weakly_suplearn_subtask1 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/weakly_suplearn_subtask1_outputs/:/workspace/outputs/ gospelslave/weakly_suplearn_subtask1:latest /bin/bash -c "sh predict.sh"

```



- [Subtask 2:  15% labels](https://hub.docker.com/repository/docker/gospelslave/weakly_suplearn_subtask2)

Run

```

# pull docker image from hub.docker.com
docker pull gospelslave/weakly_suplearn_subtask2

# docker predict command
docker container run --gpus "device=1" --name weakly_suplearn_subtask2 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/weakly_suplearn_subtask2_outputs/:/workspace/outputs/ gospelslave/weakly_suplearn_subtask2:latest /bin/bash -c "sh predict.sh"

```





- [Subtask 3:  30% labels](https://hub.docker.com/repository/docker/gospelslave/weakly_suplearn_subtask3)

Run

```

# pull docker image from hub.docker.com
docker pull gospelslave/weakly_suplearn_subtask3

# docker predict command
docker container run --gpus "device=1" --name weakly_suplearn_subtask3 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/weakly_suplearn_subtask3_outputs/:/workspace/outputs/ gospelslave/weakly_suplearn_subtask3:latest /bin/bash -c "sh predict.sh"

```





## CRF Postprocessing

Set [input and output path](https://github.com/JunMa11/AbdomenCT-1K/blob/87fc206b70bbbdbb79a0e260bb9f9106d2feb555/WeaklySupervisedLearning/SimpleCRF-master/examples/AbdominalOrganCRFSeg.py#L27-L29) and run

`python AbdominalOrganCRFSeg.py`



## Reference

[1] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, "nnU-Net: a self-confifiguring method for deep learning" based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021. 

[2] P. Krahenbuhl and V. Koltun, “Efficient inference in fully connected crfs with gaussian edge potentials,” vol. 24, pp. 109–117, 2011.

[3] M. Gao, Z. Xu, L. Lu, A. Wu, I. Nogues, R. M. Summers, and D. J. Mollura, “Segmentation label propagation using deep convolutional neural networks and dense conditional random field,” in 2016 IEEE 13th International Symposium on Biomedical Imaging, pp. 1265–1268, 2016.



## Acknowledgements

We highly appreciate the out-of-the-box implementation of [nnU-Net](https://github.com/MIC-DKFZ/nnUNet) (by [Dr. Fabian Isensee](https://github.com/FabianIsensee)) and [CRF](https://github.com/HiLab-git/SimpleCRF) (by [Dr. Guotai Wang](https://github.com/taigw)).
