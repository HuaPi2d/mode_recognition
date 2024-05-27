#pragma once
#include<opencv2\opencv.hpp>
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/objdetect/objdetect.hpp>  

// 16ά�Ҷ�ͼ������
void group_extractGLCMFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile);
// 80άGaborͼ������
void group_extractGaborFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile);
// 32ά��ɫͼ������
void group_extractColorFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile);
// 144άHOGͼ������
void group_extractHOGFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile);
