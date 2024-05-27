#pragma once
#include <iostream>  
#include <string> 
#include <vector>

using namespace std;

class FeatureFiles
{
public:
	FeatureFiles* readFeatureFiles(std::vector<std::string> fileNames, int* featureNums, int trainNum, int classNum);
	char** fileNames;   // �ļ�������
	int* featureNums;   // ���ļ�����������ά��

	int classNum;       // �����Ŀ
	int picNum;       // ѵ���� (���Լ�) ��ÿ�������ĸ���

	double* featureData;    // ����ֵ feature[featureNum*sampleNum] 
	double* PCA_featureData; // PCA��ά�������ֵ
	int* Label;
	int featureNum;     // ������ά��
	int sampleNum;      // �������� [classNum * trainNum]

	// ����ļ�
	void outPutFile(char* outPutFileName);
private:
	// �����ļ����������feature����
	void readFiles();
	// �ļ���  �Ҷ�+Gabor ��Ϊ2���ļ�
	int fileNum;
};
