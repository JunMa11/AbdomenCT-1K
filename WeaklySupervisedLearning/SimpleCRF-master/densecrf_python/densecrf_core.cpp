
#include "densecrf_core.h"


MatrixXf computeUnary( const float *prob, int H, int W, int C){
    MatrixXf r(C, H*W );
    for(int k=0; k<H*W; k++ )
    {
        for(int c = 0; c < C; c++)
        {
            // Set the energy
            float p = prob[k*C + c];
            if(p<0.01)p=0.01;
            if(p>0.99)p=0.99;
            r(c,k) = -log(p);
        }
    }
    return r;
}

VectorXs dense_crf_inference(const unsigned char * img, const float * prob, int H, int W, int C,  CRFParam param)
{
    MatrixXf unary = computeUnary(prob, H, W, C);

    // Setup the CRF model
    DenseCRF2D crf(W, H, C);
    // Specify the unary potential as an array of size W*H*(#classes)
    // packing order: x0y0l0 x0y0l1 x0y0l2 .. x1y0l0 x1y0l1 ...
    crf.setUnaryEnergy( unary );

    // add a color dependent term (feature = xyrgb)
    // x_stddev = 60
    // y_stddev = 60
    // r_stddev = g_stddev = b_stddev = 20
    // weight = 10
    crf.addPairwiseBilateral(param.alpha, param.alpha, param.beta, param.beta, param.beta, img, new PottsCompatibility( param.w1 ) );
    
    // add a color independent term (feature = pixel location 0..W-1, 0..H-1)
    // x_stddev = 3
    // y_stddev = 3
    // weight = 3
    crf.addPairwiseGaussian( param.gamma, param.gamma, new PottsCompatibility( param.w2 ) );
    
    // Do map inference
    // 	MatrixXf Q = crf.startInference(), t1, t2;
    // 	printf("kl = %f\n", crf.klDivergence(Q) );
    // 	for( int it=0; it<5; it++ ) {
    // 		crf.stepInference( Q, t1, t2 );
    // 		printf("kl = %f\n", crf.klDivergence(Q) );
    // 	}
    // 	VectorXs map = crf.currentMap(Q);
    VectorXs map = crf.map(param.iter);
    return map;
}
