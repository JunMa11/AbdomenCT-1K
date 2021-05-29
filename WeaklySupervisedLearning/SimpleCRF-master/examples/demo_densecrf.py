import denseCRF
import numpy as np
from PIL import Image
import matplotlib.pyplot as plt

def densecrf(I, P, param):
    """
    input parameters:
        I    : a numpy array of shape [H, W, C], where C should be 3.
               type of I should be np.uint8, and the values are in [0, 255]
        P    : a probability map of shape [H, W, L], where L is the number of classes
               type of P should be np.float32
        param: a tuple giving parameters of CRF (w1, alpha, beta, w2, gamma, it), where
                w1    :   weight of bilateral term, e.g. 10.0
                alpha :   spatial distance std, e.g., 80
                beta  :   rgb value std, e.g., 15
                w2    :   weight of spatial term, e.g., 3.0
                gamma :   spatial distance std for spatial term, e.g., 3
                it    :   iteration number, e.g., 5
    output parameters:
        out  : a numpy array of shape [H, W], where pixel values represent class indices. 
    """
    out = denseCRF.densecrf(I, P, param) 
    return out   

def convert_label_to_probability_map(label, color_list):
    [H, W, _] = label.shape
    C = len(color_list)
    prob  = np.zeros([H, W, len(color_list)], np.float32) 
    for h in range(H):
        for w in range(W):
            ca = label[h, w, :]
            if sum(ca) == 0:
                for c in range(C):
                    prob[h, w, c] = 1.0 / C
            else:
                for c in range(C):
                    cb = color_list[c]
                    if(ca[0]==cb[0] and ca[1]==cb[1] and ca[2]==cb[2]):
                        prob[h, w, c] = 1.0
                        break
    return prob 

def colorize_label_map(label, color_list):
    [H, W] = label.shape
    out = np.zeros((H, W, 3), np.uint8)
    for h in range(H):
        for w in range(W):
            idx = label[h, w] 
            color = np.asarray(color_list[idx])
            out[h, w, :] = color
    return out

def demo_densecrf1():
    I  = Image.open('../data/brain.png')
    Iq = np.asarray(I)
    
    # load initial labels, and convert it into an array 'prob' with shape [H, W, C]
    # where C is the number of labels
    # prob[h, w, c] means the probability of pixel at (h, w) belonging to class c.
    L  = Image.open('../data/brain_mask.png')
    Lq = np.asarray(L, np.float32) / 255
    prob = Lq[:, :, :2]
    prob[:, :, 0] = 1.0 - prob[:, :, 0]

    w1    = 10.0  # weight of bilateral term
    alpha = 80    # spatial std
    beta  = 13    # rgb  std
    w2    = 3.0   # weight of spatial term
    gamma = 3     # spatial std
    it    = 5.0   # iteration
    param = (w1, alpha, beta, w2, gamma, it)
    lab = densecrf(Iq, prob, param)
    lab = Image.fromarray(lab*255)
    plt.subplot(1,3,1); plt.axis('off'); plt.imshow(I); plt.title('input image')
    plt.subplot(1,3,2); plt.axis('off'); plt.imshow(L); plt.title('initial label')
    plt.subplot(1,3,3); plt.axis('off'); plt.imshow(lab); plt.title('after dense CRF')
    plt.show()

def demo_densecrf2():
    I  = Image.open('../dependency/densecrf/examples/im3.ppm')
    Iq = np.asarray(I, np.uint8)
    
    # load initial labels, and convert it into an array 'prob' with shape [H, W, C]
    # where C is the number of labels
    # prob[h, w, c] means the probability of pixel at (h, w) belonging to class c.
    L  = Image.open('../dependency/densecrf/examples/anno3.ppm')
    Lq = np.asarray(L, np.float32)
    color_list = [[0, 128, 0], [192, 128, 0], [64, 64, 0]] 
    prob = convert_label_to_probability_map(Lq, color_list)
   
    w1    = 10.0  # weight of bilateral term
    alpha = 80    # spatial std
    beta  = 13    # rgb  std
    w2    = 3.0   # weight of spatial term
    gamma = 3     # spatial std
    it    = 5.0   # iteration
    param = (w1, alpha, beta, w2, gamma, it)
    lab = densecrf(Iq, prob, param)
    lab = colorize_label_map(lab, color_list)
    lab = Image.fromarray(lab)
    plt.subplot(1,3,1); plt.axis('off'); plt.imshow(I); plt.title('input image')
    plt.subplot(1,3,2); plt.axis('off'); plt.imshow(L); plt.title('initial label')
    plt.subplot(1,3,3); plt.axis('off'); plt.imshow(lab); plt.title('after dense CRF')
    plt.show()

if __name__ == "__main__":
    print("example list")
    print(" 0 -- Dense CRF example for a binary segmentation")
    print(" 1 -- Dense CRF example for a multi-class segmentation")
    print("please enter the index of an example:")
    method = input()
    method = "{0:}".format(method)
    if(method == '0'):
        demo_densecrf1()
    elif(method == '1'):
        demo_densecrf2()
    else:
        print("invalid number : {0:}".format(method))