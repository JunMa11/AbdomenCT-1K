#include "densecrf3d_util.h"
#include <iostream>
using namespace std;

void addPairwiseBilateral(
        DenseCRF3D & crf3d, int numberOfModalities, double BilateralRStd, double BilateralCStd, double BilateralZStd,
        std::vector<double>  BilateralModsStds, double BilateralW,
        unsigned char * arrayWithMyImWithAllModalities) {
    if (numberOfModalities==1) {
        std::cout<<"Using addPairwiseBilateral for 1 Modality\n";
        crf3d.addPairwiseBilateral1Mod(BilateralRStd,
                                       BilateralCStd,
                                       BilateralZStd,
                                       BilateralModsStds[0],
                                       arrayWithMyImWithAllModalities,
                                       new PottsCompatibility( BilateralW ) );
    } else if (numberOfModalities==2) {
        std::cout<<"Using addPairwiseBilateral for 2 Modalities\n";
        crf3d.addPairwiseBilateral2Mod(BilateralRStd,
                                       BilateralCStd,
                                       BilateralZStd,
                                       BilateralModsStds[0],
                                       BilateralModsStds[1],
                                       arrayWithMyImWithAllModalities,
                                       new PottsCompatibility( BilateralW ) );
    } else if (numberOfModalities==3) {
        std::cout<<"Using addPairwiseBilateral for 3 Modalities\n";
        crf3d.addPairwiseBilateral3Mod(BilateralRStd,
                                       BilateralCStd,
                                       BilateralZStd,
                                       BilateralModsStds[0],
                                       BilateralModsStds[1],
                                       BilateralModsStds[2],
                                       arrayWithMyImWithAllModalities,
                                       new PottsCompatibility( BilateralW ) );
    } else if (numberOfModalities==4) {
        std::cout<<"Using addPairwiseBilateral for 4 Modalities\n";
        crf3d.addPairwiseBilateral4Mod(BilateralRStd,
                                       BilateralCStd,
                                       BilateralZStd,
                                       BilateralModsStds[0],
                                       BilateralModsStds[1],
                                       BilateralModsStds[2],
                                       BilateralModsStds[3],
                                       arrayWithMyImWithAllModalities,
                                       new PottsCompatibility( BilateralW ) );
    } else if (numberOfModalities==5) {
        std::cout<<"Using addPairwiseBilateral for 5 Modalities\n";
        crf3d.addPairwiseBilateral5Mod(BilateralRStd,
                                       BilateralCStd,
                                       BilateralZStd,
                                       BilateralModsStds[0],
                                       BilateralModsStds[1],
                                       BilateralModsStds[2],
                                       BilateralModsStds[3],
                                       BilateralModsStds[4],
                                       arrayWithMyImWithAllModalities,
                                       new PottsCompatibility( BilateralW ) );
    }
}