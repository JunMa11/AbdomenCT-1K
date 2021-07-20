#include "densecrf.h"

void addPairwiseBilateral(
        DenseCRF3D & crf3d, int numberOfModalities, double BilateralRStd, double BilateralCStd, double BilateralZStd,
        std::vector<double>  BilateralModsStds, double BilateralW,
        unsigned char * arrayWithMyImWithAllModalities);