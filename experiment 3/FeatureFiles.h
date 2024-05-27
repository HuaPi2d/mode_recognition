#pragma once
#include <iostream>  
#include <string> 
#include <vector>

using namespace std;

class FeatureFiles
{
public:
	FeatureFiles* readFeatureFiles(std::vector<std::string> fileNames, int* featureNums, int trainNum, int classNum);
	char** fileNames;   // 文件名集合
	int* featureNums;   // 各文件包含的特征维数

	int classNum;       // 类别数目
	int picNum;       // 训练集 (测试集) 中每类样本的个数

	double* featureData;    // 特征值 feature[featureNum*sampleNum] 
	double* PCA_featureData; // PCA降维后的特征值
	int* Label;
	int featureNum;     // 总特征维数
	int sampleNum;      // 样本总数 [classNum * trainNum]

	// 输出文件
	void outPutFile(char* outPutFileName);
private:
	// 读入文件，生成组合feature数组
	void readFiles();
	// 文件数  灰度+Gabor 即为2个文件
	int fileNum;
};
