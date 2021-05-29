#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Mar 21 14:27:55 2021

@author: mia
"""
import nibabel as nib
import denseCRF3D
import numpy as np
from medpy import metric
import os
join = os.path.join


def densecrf3d(I, P, param):
    """
    input parameters:
        I: a numpy array of shape [D, H, W, C], where C is the channel number
           type of I should be np.uint8, and the values are in [0, 255]
        P: a probability map of shape [D, H, W, L], where L is the number of classes
           type of P should be np.float32
        param: a tuple giving parameters of CRF. see the following two examples for details.
    """
    return denseCRF3D.densecrf3d(I, P, param)

img_path = 'path to image'
seg_path = 'path to prediction npz file'
save_path = 'path to CRF segmentation results'

names = sorted(os.listdir(gt_path))
for name in names:
    try:
        name_pre = name.split('.nii.gz')[0]
        img_nii = nib.load(join(img_path, name_pre + '_0000.nii.gz'))
        img_data = img_nii.get_fdata()
        img_data[img_data>240] = 240
        img_data[img_data<-160] = -160
        img_data = (img_data+160)/400 *255
        # read initial segmentation
        ini_seg_nii = nib.load(join(seg_path, name))
        ini_seg_data = np.uint8(ini_seg_nii.get_fdata())

        # read initial softmax
        ini_seg_npz = np.load(join(seg_path, name_pre+'.npz'))
        ini_seg = ini_seg_npz.f.softmax
        
        I = np.asarray(np.expand_dims(img_data,-1), np.uint8)
        
        P = np.transpose(ini_seg, [3, 2, 1, 0])
        
        dense_crf_param = {}
        dense_crf_param['MaxIterations'] = 5.0 # 10, 15
        dense_crf_param['PosW'] = 2.0
        dense_crf_param['PosRStd'] = 5
        dense_crf_param['PosCStd'] = 5
        dense_crf_param['PosZStd'] = 5
        dense_crf_param['BilateralW'] = 3.0
        dense_crf_param['BilateralRStd'] = 5.0
        dense_crf_param['BilateralCStd'] = 5.0
        dense_crf_param['BilateralZStd'] = 5.0
        dense_crf_param['ModalityNum'] = 1
        dense_crf_param['BilateralModsStds'] = (5.0,)
        
        refine_seg = densecrf3d(I, P, dense_crf_param)
        save_Nii = nib.Nifti1Image(refine_seg, img_nii.affine, img_nii.header)
        nib.save(save_Nii, join(save_path, name))

    except:
        print('*'*30)
        print(name, 'failed!!!')
        print('*'*30)




