#pragma once
#include<iostream>  
#include<cmath>  
#include<opencv2/opencv.hpp>

using namespace std;

void BP_Classify(double traindata[], int trainLabel[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double *test_P, double* train_P);