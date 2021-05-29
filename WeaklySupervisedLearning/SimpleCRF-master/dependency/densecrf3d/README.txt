# Author: Konstantinos Kamnitsas (kk2412@imperial.ac.uk)
# last modified: 27/May/2016

# This is a 3D Fully Connected CRF, made for easily processing 3D Volumetric scans. The method is highly efficient, capable of processing a mono-modal brain scan in under a minute, and multi-modal scans in 2-3 minutes, when run on the CPU. 

# The original CRF of Krähenbühl and Koltun (website: http://www.philkr.net/home/densecrf) was extended to 3D, for the processing of 3D volumetric scans. 
# The ITK library was integrated for the loading/saving of NIFTI files. 
# This 3D CRF can process multi-modal NIFTI scans, if it is given unary potentials (probability maps) for the classes.
# It is capable of multi-class problems.

# This version of the CRF for application on biomedical 3D volumes was developed and used for our works :
# [1] Kamnitsas et al in "Efficient Multi-Scale 3D CNN with Fully Connected CRF for Accurate Brain Lesion Segmentation", arXiv preprint, arXiv:1603.05959, 2016.
# [2] Kamnitsas et al in "Multi-Scale 3D Convolutional Neural Networks for Lesion Segmentation in Brain MRI", in Proceeding of ISLES challenge, MICCAI, 2015, winning submission.

# If you are using this software, please cite the amazing work of Krähenbühl and Koltun, on which this project was based: 
# [3] Philipp Krähenbühl and Vladlen Koltun, "Efficient inference in fully connected crfs with gaussian edge potentials", in NIPS, 2011.

# If you find this software helps you in your research, also consider citing our work [1], where the first successful application of this 3D version of the CRF on biomedical data was presented.

# *********** License ************
# This software is licensed under the BSD license. A copy of the license is provided at the top of each source file.
# The data provided for the accompanying examples are released under Open Database License (ODbL). Copy of the license can be found along with the data files.

# *********** How to build it ************
# Requires CMake and ITK (https://itk.org/).
# > "mkdir build" in the main folder. 
# > "cd build"
# > "cmake .."
# > "make"
# The binary "dense3DCrfInferenceOnNiis" should now be in the /build/applicationAndExamples/ folder, which is the main application.

# ************ How to run it ***********
# Application can be run either
# a) with a config file: use option "-c" followed by the config-file.
# b) by passing all following input parameters straight from the command line. But the parameters are many, so config file is highly suggested.

# As an example, from within the "build" folder run: ./applicationAndExamples/dense3DCrfInferenceOnNiis -c ../applicationAndExamples/example/configFileDenseCrf3d.txt

# Results should appear in the applicationAndExamples/example/results/ folder.
# As a template config file, this very readme file can be used. Below, is the explanation of all the parameters... 


# ************ Requirements for running the 3D CRF **************
# - All the channels (modalities) should have the same image-dimension and size, and co-registered in image space.
# - Positional and bilateral std should be positives.
# - Currently can process up to 5 modalities. Easily extendable to more (see src/densecrf.cpp).
# - The various channels should be normalised to the same intensity range. A min and max intensity range is required to be given. Values below or above this will be set to the boundary value.
# - Some care when filling this config file! See notes below!
# - The arguments for each option should be given in separate, sequential lines. No empty lines or comments between an option and its corresponding value, or my custom parser breaks!
# - A few minutes waiting time, for the inference :)
# - The method has quite some parameters to configure. Manual configuration or grid/random search is common practice. Hopefully we ll update it with a learning method soon.

# IMPORTANT NOTE: The -option and the corresponding values should be placed in sequential lines currently. No white lines or comments between.


#-------BELOW ARE THE CONFIG FILE PARAMETERS TO RUN THE 3D CRF INFERENCE. This whole README file can be used as a config file! ------

#-------------------- INPUT PARAMETERS --------------------

# numberOfModalitiesAndFiles: (int) number of modalities to use, followed by the full paths-filenames of the corresponding Nifti (or nii.gz) files.
-numberOfModalitiesAndFiles
2
../applicationAndExamples/example/DWI_normalized.nii.gz
../applicationAndExamples/example/Flair_normalized.nii.gz

# numberOfForegroundClassesAndProbMapFiles: (int) number of FOREGROUND classes, followed by full paths-filenames to the corresponding probability maps.
# The prob-maps will be used for unary potentials!
-numberOfForegroundClassesAndProbMapFiles
1
../applicationAndExamples/example/lesionProbMap.nii.gz

# imageDimensions: (int) the dimensions of the image (2 for 2D, 3 for 3D)
# (TODO: This version should work for 2D too. But there is a bug in my config-parser that will not allow it >.< Will fix soon! So keep first value == 3 for 3D now!)
# This should be followed by the dimension of the image in the corresponding R-C-Z axes (everything in separate lines, my config-file-parser is bad!)
-imageDimensions
3
230
230
154

# minMaxIntensities: (float) min intensity, and followed by max intensity (in separate lines).
# All the channels (modalities) should have already been normalised to the same intensity range. The min and max values to use should be given here.
# Every value below or above these boundaries will be set the to min/max respectively.
-minMaxIntensities
-3.1
+3.1

#-------------------- OUTPUT PARAMETERS --------------------

# outputFolder: output folder, where the results should be saved (segmentation maps and probability maps generated by the CRF). NOTE: Folder should have been created beforehand!
-outputFolder
../applicationAndExamples/example/results/

# prefixForOutputSegmentationMap: Essentially the filename for the resulting segmentation map (default is denseCrf3dOutputSegm). Will be saved as a .nii.gz automatically.
-prefixForOutputSegmentationMap
denseCrf3dSegmMap

# prefixForOutputProbabilityMaps: Prefix of the filenames with which to save the resulting probability maps (default is denseCrf3dProbMapClass).
# Each probability map will be saved as "prefix" + numberOfClass + ".nii.gz" automatically.
-prefixForOutputProbabilityMaps
denseCrf3dProbMapClass

#------------------- HERE ARE THE CRF PARAMETERS----------------------

# pRCZandW: please provide 4 sequential floats (separate lines) for pR, pC, pZ, pW.
# positional-std parameters and the corresponding weight. The higher the stds, the larger the neighbourhood that the pixel is influenced by the nearby pixel-labels.
# Similary, higher positional-W means the energy function will require nearby voxels to have consistent labels.
-pRCZandW
3.0
3.0
3.0
3.0

# bRCZandW: please provide 4 floats (separate lines) for bR, bC, bZ, bW.
# bilateral-std parameters (bRCZ) and the corresponding weight (bW). bilateral RCZ are similar to the positional RCZ parameters. 
# But these contribute to the hybrid kernel, that is also influenced by the intensities in the images (-bMods section below). See original paper for more details.
# bW the bilateral weight that defines the influence of the hybrid kernel (distance of pixel + intensity difference).
# (Note: Imagine this as defining how far away from a pixel we require the intensities in the values to be rather homogeneous. bW values used were between 3-30, depends on number of modalities used too.)
-bRCZandW
17.0
12.0
10.0
5.0

# bMods: as many floats as the channels (modalities) used (in separate sequential lines). 
# These bilateral stds define how much intensity-homogeneity is required within a region.
# Higher values allow greater variations in the corresponding channel under the same label.
-bMods
4.5
3.5

# numberOfIterations: How many times the CRF regularisation will be iteratively performed. Common choices are 5-10. But it is worth some experimentation for faster inference. 
-numberOfIterations
2

