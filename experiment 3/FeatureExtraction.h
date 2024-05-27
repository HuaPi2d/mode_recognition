#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;

class FeatureExtraction
{
public:
	char* bmpFileRoot;				// ���ݼ���·��
	int classNum;					// �����
	int begin;						// ��ʼͼ�����
	int end;						// ��ֹͼ�����
	char* featureOutputFileRoot;	// ����ļ���·��
	
	void group_extractFeature(int index); 
	void group_extractGLCMFeature();      // ��ȡ16ά�Ҷȹ����������� index = 0
	void group_extractGaborFeature();     // ��ȡ80άGabor���� index = 1
	void group_extractCOLORFeature();     // ��ȡ5ά��ɫ���� index = 2
	void group_extractSHAPEFeature();     // ��ȡ6ά��״���� index = 3
private:
	char* featureOutputFileName;	// ����ļ���
	char* featureOutputFile;		// ����ļ�ȫ·��
};

