import nibabel
import denseCRF3D
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

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

def demo_densecrf3d_1():
    I1Nii = nibabel.load('../data/2013_12_1_img.nii.gz')
    PNii  = nibabel.load('../data/2013_12_1_init.nii.gz')
    I1 = I1Nii.get_data()
    P  = PNii.get_data()

    # convert input to intenstiy range of [0, 255]
    I = np.asarray([I1], np.float32)
    I = np.transpose(I, [1, 2, 3, 0])
    I = I / I.max()* 255
    I = np.asarray(I, np.uint8)

    # probability map for each class
    P = 0.5 + (P - 0.5) * 0.8
    P = np.asarray([1.0 - P, P], np.float32)
    P = np.transpose(P, [1, 2, 3, 0])

    dense_crf_param = {}
    dense_crf_param['MaxIterations'] = 2.0
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

    lab = densecrf3d(I, P, dense_crf_param)
    labNii = nibabel.Nifti1Image(lab, np.eye(4))
    nibabel.save(labNii, '../data/seg_densecrf.nii.gz')

def demo_densecrf3d_2():
    data_root  = '../dependency/densecrf3d/applicationAndExamples/example/'
    I1Nii = nibabel.load(data_root + 'Flair_normalized.nii.gz')
    I2Nii = nibabel.load(data_root + 'DWI_normalized.nii.gz')
    PNii  = nibabel.load(data_root + 'lesionProbMap.nii.gz')
    I1 = I1Nii.get_data()
    I2 = I2Nii.get_data()
    P  = PNii.get_data()

    # convert input normalized image to intenstiy range of [0, 255]
    I = np.asarray([I1, I2], np.float32)
    I = np.transpose(I, [1, 2, 3, 0])
    I = (I + 3)/6.0 * 255
    I[I < 0] = 0
    I[I > 255] = 255
    I = np.asarray(I, np.uint8)

    # probability map for each class
    P = np.asarray([1.0 - P, P], np.float32)
    P = np.transpose(P, [1, 2, 3, 0])

    dense_crf_param = {}
    dense_crf_param['MaxIterations'] = 2.0
    dense_crf_param['PosW'] = 2.0
    dense_crf_param['PosRStd'] = 5
    dense_crf_param['PosCStd'] = 5
    dense_crf_param['PosZStd'] = 5
    dense_crf_param['BilateralW'] = 3.0
    dense_crf_param['BilateralRStd'] = 5.0
    dense_crf_param['BilateralCStd'] = 5.0
    dense_crf_param['BilateralZStd'] = 5.0
    dense_crf_param['ModalityNum'] = 2
    dense_crf_param['BilateralModsStds'] = (5.0, 5.0)

    lab = densecrf3d(I, P, dense_crf_param)
    labNii = nibabel.Nifti1Image(lab, np.eye(4))
    nibabel.save(labNii, data_root + 'results/lesionSegMap.nii.gz')

if __name__ == "__main__":
    print("example list")
    print(" 0 -- 3D Dense CRF example for single-modal segmentation")
    print(" 1 -- 3D Dense CRF example for multi-modal segmentation")
    print("please enter the index of an example:")
    method = input()
    method = "{0:}".format(method)
    if(method == '0'):
        demo_densecrf3d_1()
    elif(method == '1'):
        demo_densecrf3d_2()
    else:
        print("invalid number : {0:}".format(method))


