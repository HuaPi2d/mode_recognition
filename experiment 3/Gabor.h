#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

void  getAllGaborKernal(int GaborNum, int U, int V, Mat kel_GR[], Mat kel_GI[]);
void  makeGaborKernal(int ke_h, int ke_w, int u, int v, Mat &GaborReal, Mat &GaborImg);

//Gabor提特征，对外函数
void  calculate_Feature(Mat &src, Mat kel_GR[], Mat kel_GI[], int GaborNum, double feature[]);
