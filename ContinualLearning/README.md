# Weakly Supervised Learning Baseline

## Introduction

The baseline is built on the combination of 2D nnUNet [1] and fully connected Conditional Random Fields (CRF)[2], which is motivated by Gao et al.'s method in addressing the missing annotation problem [3].



## Training

Run

`nnUNet_train 2d nnUNetTrainerV2 TaskXXX_MYTASK all`



## Inference

Run

`nnUNet_predict -i INPUT_FOLDER -o OUTPUT_FOLDER -t TASK_NAME_OR_ID -m 2d --save_npz`



## CRF Postprocessing







## Reference

[1] F. Isensee, P. F. Jaeger, S. A. A. Kohl, J. Petersen, and K. H. Maier-Hein, "nnU-Net: a self-confifiguring method for deep learning" based biomedical image segmentation,” Nature Methods, vol. 18, no. 2, pp. 203–211, 2021. 

[2] P. Krahenbuhl and V. Koltun, “Efficient inference in fully connected crfs with gaussian edge potentials,” vol. 24, pp. 109–117, 2011.

[3] M. Gao, Z. Xu, L. Lu, A. Wu, I. Nogues, R. M. Summers, and D. J. Mollura, “Segmentation label propagation using deep convolutional neural networks and dense conditional random field,” in 2016 IEEE 13th International Symposium on Biomedical Imaging, pp. 1265–1268, 2016.



## Acknowledgements

We highly appreciate the out-of-the-box implementation of [nnU-Net](https://github.com/MIC-DKFZ/nnUNet) (by [Dr. Fabian Isensee](https://github.com/FabianIsensee)) and [CRF](https://github.com/HiLab-git/SimpleCRF) (by [Dr. Guotai Wang](https://github.com/taigw)).
