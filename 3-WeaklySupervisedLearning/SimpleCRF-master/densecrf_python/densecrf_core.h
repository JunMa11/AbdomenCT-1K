#include "densecrf.h"
#include <cstdio>
#include <stdio.h>
#include <cmath>
#include <iostream>
using namespace std;
const float GT_PROB = 0.5;

struct CRFParam{
    double w1=0;
    double alpha=0;
    double beta=0;
    double w2=0;
    double gamma=0;
    int iter=0;
};
// VectorXs dense_crf_inference(const unsigned char * img, const float * prob, int W, int H, CRFParam param);
VectorXs dense_crf_inference(const unsigned char * img, const float * prob, int H, int W, int C, CRFParam param);