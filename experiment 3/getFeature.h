#pragma once
#include<opencv2\opencv.hpp>
#include <opencv2/imgcodecs.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/objdetect/objdetect.hpp>  

// 16Î¬»Ò¶ÈÍ¼ÏñÌØÕ÷
void group_extractGLCMFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile);
// 80Î¬GaborÍ¼ÏñÌØÕ÷
void group_extractGaborFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile);
// 32Î¬ÑÕÉ«Í¼ÏñÌØÕ÷
void group_extractColorFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile);
// 144Î¬HOGÍ¼ÏñÌØÕ÷
void group_extractHOGFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile);
