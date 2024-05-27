#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class FeatureExtraction
{
public:
	char* bmpFileRoot;				// 数据集根路径
	int classNum;					// 类别数
	int begin;						// 起始图像序号
	int end;						// 终止图像序号
	char* featureOutputFileRoot;	// 输出文件根路径
	
	void group_extractFeature(int index); 
	void group_extractGLCMFeature();      // 提取16维灰度共生矩阵特征 index = 0
	void group_extractGaborFeature();     // 提取80维Gabor特征 index = 1
	void group_extractCOLORFeature();     // 提取5维颜色特征 index = 2
	void group_extractSHAPEFeature();     // 提取6维形状特征 index = 3
private:
	char* featureOutputFileName;	// 输出文件名
	char* featureOutputFile;		// 输出文件全路径
};

