The fully supervised benchmark is served as the official [MICCAI 2021 Challenge](https://www.miccai2021.org/en/MICCAI2021-CHALLENGES.html): Fast and Low GPU Memory Abdominal Organ Segmentation challenge ([Homepage](https://flare.grand-challenge.org/FLARE21/))



## Baseline Docker

- [Subtask 1](https://hub.docker.com/repository/docker/gospelslave/fully_suplearn_subtask1)

Run

`docker pull gospelslave/fully_suplearn_subtask1`

`docker container run --gpus "device=1" --name fully_suplearn_subtask1 --rm -v $PWD/TestImage/:/workspace/input/ -v $PWD/fully_suplearn_subtask1_outputs/:/workspace/outputs/ gospelslave/fully_suplearn_subtask1:latest /bin/bash -c "sh predict.sh"`



