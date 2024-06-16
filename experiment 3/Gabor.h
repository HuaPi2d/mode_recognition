#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

void  getAllGaborKernal(int GaborNum, int U, int V, cv::Mat kel_GR[], cv::Mat kel_GI[]);
void  makeGaborKernal(int ke_h, int ke_w, int u, int v, cv::Mat &GaborReal, cv::Mat &GaborImg);

//Gabor提特征，对外函数
void  calculate_Feature(cv::Mat &src, cv::Mat kel_GR[], cv::Mat kel_GI[], int GaborNum, double feature[]);
