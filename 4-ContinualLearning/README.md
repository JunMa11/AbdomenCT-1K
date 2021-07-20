# Continual Learning Baseline

## Introduction

The baseline is movitated by the well-known  learning without forgetting [1], which contains the following four steps:

- Step 1. Individually training a liver segmentation nnU-Net [2] model based on the MSD Pancreas Ts (139) dataset.
- Step 2. Using the trained liver segmentation model to infer KITS (210) and obtaining pseudo liver labels. Thus, each case in the KITS (210) has both liver and kidney labels. Then, we use the new labels to train a nnU-Net model that can segment both liver and kidney.
- Step 3. Using the trained model in Step 2 to infer Spleen (41) and obtaining both liver and kidney pseudo labels. Thus, each case in the Spleen (210) has liver, kidney, and spleen labels. Then, we use the new labels to train a nnU-Net model that can segment the three organs.
- Step 4. Using the trained model in Step 3 to infer MSD Pancreas (281) and obtaining both liver, kidney and spleen pseudo labels. Thus, each case in the MSD Pancreas (281) has liver, kidney, spleen, and pancreas labels. Finally, we can obtain the final multi-organ segmentation model by training a nnU-Net with the new labels.



## Training

Run

`nnUNet_train 3d_fullres nnUNetTrainerV2 Task441_ContinualLiver all`

`nnUNet_train 3d_fullres nnUNetTrainerV2 Task442_ContinualLiverKidney all`

`nnUNet_train 3d_fullres nnUNetTrainerV2 Task443_ContinualLivKidSpl all`

`nnUNet_train 3d_fullres nnUNetTrainerV2 Task444_ContinualFourOrgans all`

## Trained Models

- [Dropbox](https://www.dropbox.com/s/ffm3aay6g9hv15s/BaselineModels.zip?dl=0)
- [Baidu Net Disk](https://pan.baidu.com/s/1eZIFUav-YFiD4U5azFqVEg) (PW:2021)

## Inference

Run

`nnUNet_predict -i Test_FOLDER -o OUTPUT_FOLDER -t Task444_ContinualFourOrgans -m 3d_fullres -f all --disable_tta`

## Baseline Docker

- [continual_learn](https://hub.docker.com/repository/docker/gospelslave/continual_learn)

Run

```

# pull docker image from hub.docker.com
docker pull gospelslave/continual_learn

# start command
docker container run --gpus "device=1" --name continual_learn --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/continual_learn_outputs/:/workspace/outputs/ gospelslave/continual_learn:latest /bin/bash -c "sh predict.sh"

```





## Reference
[1] Z. Li and D. Hoiem, “Learning without forgetting,” IEEE Transactions on Pattern Analysis and Machine Intelligence, vol. 40, no. 12, pp. 2935–2947, 2017.

[2] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, "nnU-Net: a self-confifiguring method for deep learning" based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021. 







## Acknowledgements

We highly appreciate the out-of-the-box implementation of [nnU-Net](https://github.com/MIC-DKFZ/nnUNet) (by [Dr. Fabian Isensee](https://github.com/FabianIsensee)).
