#pragma once
#include<opencv2\opencv.hpp>

using namespace std;
using namespace cv::ml;

void SVM_Classify(double traindata[], int trainLabel[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double P[][20]);