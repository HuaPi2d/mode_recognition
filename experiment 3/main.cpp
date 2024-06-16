#pragma warning(disable:4996)
#define WIN32_LEAN_AND_MEAN

#include <iostream>  
#include <opencv2/opencv.hpp>
#include <cmath>  
#include <vector>
#include <string>
#include <engine.h>
#include "Normal_Bayes_Classifier.h"
#include "c_SVM.h"
#include "Gabor.h"
#include "FeatureFiles.h"
#include "getFeature.h"
#include "proprint.h"
#include "k-means.h"
#include "BP.h"
#include "paint_result.h"
#include "./paintVsualize.h"
#include "./hierarchicalClustering_packed.h"
#include "calculate_intra_inter_packed.h"
#include "dbscan.h"


//�����С��һ����ÿ������һ�н��й�һ��������������ѵ�������������С��һ��
void normal(double traindata[], int trainSampleNum, int featureNum, double testdata[], int testSampleNum)
{
	double *train_featureData_max_min=new double[featureNum*2];
	for (int i = 0; i < featureNum; i++) {//�ҵ�ÿά���������ֵ����Сֵ
		double tmp_max, tmp_min;
		tmp_max = traindata[0*featureNum +i];
		tmp_min = traindata[0 * featureNum + i];
		for (int j = 1; j < trainSampleNum; j++) {
			if (tmp_max < traindata[j*featureNum+i])
				tmp_max = traindata[j*featureNum + i];
			if (tmp_min > traindata[j*featureNum + i])
				tmp_min = traindata[j*featureNum + i];
		}
		train_featureData_max_min[i*2+0] = tmp_max;
		train_featureData_max_min[i*2+1] = tmp_min;
	}
	//��ѵ��������һ��������ӳ�䵽0-1֮��
	for (int j = 0; j < featureNum; j++){
		for (int i = 0; i < trainSampleNum; i++) 
			if (fabs(train_featureData_max_min[j * 2 + 0] - train_featureData_max_min[j * 2 + 1]) < 1E-7) {
				traindata[i * featureNum + j] = 0;
			}
			else {
				traindata[i*featureNum +j] =(traindata[i*featureNum +j] - train_featureData_max_min[j*2+1]) / (train_featureData_max_min[j*2+0] - train_featureData_max_min[j*2+1]);
			}
	}
	
	//������������һ������ѵ�������������Сֵ�����п��ܲ���0-1֮��
	for (int j = 0; j < featureNum; j++) {
    	for (int i = 0; i < testSampleNum; i++)
			if (fabs(train_featureData_max_min[j * 2 + 0] - train_featureData_max_min[j * 2 + 1]) < 1E-7) {
				if (testdata[i * featureNum + j] != train_featureData_max_min[j * 2 + 0]) {
					testdata[i * featureNum + j] = 1;
				}
			}
			else {
				testdata[i * featureNum + j] = (testdata[i * featureNum + j] - train_featureData_max_min[j * 2 + 1]) / (train_featureData_max_min[j * 2 + 0] - train_featureData_max_min[j * 2 + 1]);
			}
	}
}

// �����Թ�һ��
void abnormal(double traindata[], int trainSampleNum, int featureNum, double testdata[], int testSampleNum) {
	double* mu = new double[featureNum];
	double sumj;
	double* sigma = new double[featureNum];

	// ���jά�����ľ�ֵ
	for (int j = 0; j < featureNum; j++) {
		sumj = 0;
		for (int i = 0; i < trainSampleNum; i++) {
			sumj += traindata[i * featureNum + j];
		}
		mu[j] = sumj / trainSampleNum;
	}

	// ���jά�����ı�׼��
	for (int j = 0; j < featureNum; j++) {
		sumj = 0;
		for (int i = 0; i < trainSampleNum; i++) {
			sumj += (traindata[i * featureNum + j] - mu[j]) * (traindata[i * featureNum + j] - mu[j]);
		}
		sigma[j] = sqrt(sumj / trainSampleNum);
	}

	// ����������һ��
	for (int j = 0; j < featureNum; j++) {
		for (int i = 0; i < trainSampleNum; i++) {
			traindata[i * featureNum + j] = ((traindata[i * featureNum + j]) / (3 * sigma[j]) + 1) / 2; //�����Թ�һ����ʽ
		}
	}

	// ѵ��������һ��
	for (int j = 0; j < featureNum; j++) {
		for (int i = 0; i < testSampleNum; i++) {
			testdata[i * featureNum + j] = ((testdata[i * featureNum + j]) / (3 * sigma[j]) + 1) / 2;
		}
	}
}

//trainDataInΪѵ��������testDataInΪ����������pcaFeatureNumΪ��ά������ά����trainDataOutΪ��ά��ѵ������, testDataOutΪ��ά���������
void PCA_reduct_dimension(double trainDataIn[], double testDataIn[], int classNum, int trainNum, int testNum, int featureNum, int pcaFeatureNum, double trainDataOut[], double testDataOut[])
{
	//�Ѷ��������ת����cv::Mat���ͣ���һά�����������ڶ�ά��������
	// cout << classNum << ' ' << trainNum << ' ' << featureNum << endl;
	cv::Mat trainDataIn_Mat(classNum*trainNum, featureNum, CV_64FC1);
	for (int i = 0; i < classNum*trainNum; i++) {
		for (int j = 0; j < featureNum; j++) {
			trainDataIn_Mat.at<double>(i, j) = trainDataIn[i*featureNum + j];//�����ݸ��Ƶ�trainDataIn_cv::Mat
		}
	}
	//PCA�ֽ�õ���������������
	//trainDataIn_cv::Mat����Ҫ��ά�����ݣ�cv::Mat()�Ǹ������ݣ���ϵͳ�Զ����������ݼ����ֵ
	//CV_PCA_DATA_AS_ROW����־������������һ����һ��������pcaFeatureNum��ά������ά������������ά����ͬ
	cv::PCA pca(trainDataIn_Mat, cv::Mat(), cv::PCA::DATA_AS_ROW, pcaFeatureNum);//cv::PCA::DATA_AS_ROW  CV_PCA_DATA_AS_ROW

	//����������pca.eigenvectorsת��
	transpose(pca.eigenvectors, pca.eigenvectors);
	//���㽵ά����������
	cv::Mat trainDataOut_Mat = trainDataIn_Mat * pca.eigenvectors;//�������
	//cout << pca.eigenvectors.cols << endl;

	//�Բ������ݽ�ά������ѵ�����������������󣬼ٶ����ǲ������ݺ�ѵ�����ݷ�����ͬ�ֲ�
	//�Ѷ��������ת����Mat���ͣ���һά�����������ڶ�ά��������
	cv::Mat testDataIn_Mat(classNum*testNum, featureNum, CV_64FC1);
	for (int i = 0; i < classNum*testNum; i++) {
		for (int j = 0; j < featureNum; j++) {
			testDataIn_Mat.at<double>(i, j) = testDataIn[i*featureNum + j];
		}
	}

	cv::Mat testDataOut_Mat = testDataIn_Mat * pca.eigenvectors;//�������ݽ�ά�������
	//cout << testDataOut_cv::Mat.cols << endl;

	//��ѵ�����Ͳ��Լ�PCA��ά������תΪdouble��������
	for (int i = 0; i < classNum*trainNum; i++) {
		for (int j = 0; j < pcaFeatureNum; j++) {
			trainDataOut[i*pcaFeatureNum + j] = trainDataOut_Mat.at<double>(i, j);
		}
	}
	for (int i = 0; i < classNum*testNum; i++) {
		for (int j = 0; j < pcaFeatureNum; j++) {
			testDataOut[i*pcaFeatureNum + j] = testDataOut_Mat.at<double>(i, j);
		}
	}
}

//ͳ��׼ȷ��
double calculate_accuracy(double* P, int test_Label[], int classNum, int testNum)
{
	double accuracy;
	int correct_P = 0;//��ȷԤ��ĸ���
	for (int k = 0; k < classNum * testNum; k++) {
		double tmp_max_p;
		int prediction_L = 1;//Ԥ�����
		tmp_max_p = P[k * classNum];
		for (int c = 0; c < classNum; c++) {
			if (tmp_max_p < P[k*classNum+c]) {
				tmp_max_p = P[k * classNum + c];
				prediction_L = c + 1;
			}
		}
		//printf("%d %d\n", prediction_L, test_Label[k]);
		if (prediction_L == test_Label[k])
			correct_P++;
		else {
			// cout << "���������\n" << "�ࣺ" << k / testNum + 1 << " ������ţ�" << k % testNum + 17 << " ���Ϊ�ࣺ" << prediction_L << endl;
		}
	}
	accuracy = double(correct_P) / (classNum * testNum); //�����Ԥ��׼ȷ��
	return accuracy;
}

// cluster accuracy ����׼ȷ��
double accuracy(int* Labelarray, int classNum, int picNum) {
	int* class_Num = new int[classNum * 2];
	int right_Num = 0;
	int max;
	for (int i = 0; i < classNum; i++) {
		// ��ʼ��
		for (int j = 0; j < classNum * 2; j++) {
			class_Num[j] = 0;
		}
		max = 0;

		// ����һ����������������Ŀ
		for (int k = 0; k < picNum; k++) {
			if (Labelarray[i * classNum + k] == -2) {
				continue;
			}
			class_Num[Labelarray[i * classNum + k]]++;
			if (class_Num[Labelarray[i * classNum + k]] > max) {
				max = class_Num[Labelarray[i * classNum + k]];
			}
		}

		// ����ÿ�������е���ȷԤ�����Ŀ
		cout << "���" << i << ':'  << max << endl;
		right_Num += max;
	}
	return (right_Num * 1.0) / (classNum * picNum);
}

// PCA���Ż�
void PCA_optimize(int featureNum, int step, FeatureFiles* trainFf, FeatureFiles* testFf) 
{
	double* P; //�������Ԥ�����
	double accuracy; // ׼ȷ��

	// ԭʼ��������ά
	if (featureNum > 80) {
		featureNum = 80;
	}
	else {
		P = new double[testFf->classNum * testFf->picNum * testFf->classNum];
		//bayesClassify(train_featureData, test_featureData, classNum, trainNum, testNum, featureNum, P);
		bayesClassify(trainFf->featureData, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, featureNum, P);
		//cout_2D_array(P, testFf->classNum * testFf->picNum, testFf->classNum);
		//׼ȷ�ʼ���
		accuracy = calculate_accuracy(P, testFf->Label, testFf->classNum, testFf->picNum);
		// printf("���Լ���СΪ��%d\n", classNum*testNum);
		cout << "ԭʼ��������ά��Ϊ" << featureNum;
		printf("Ԥ��׼ȷ��Ϊ��%f\n", accuracy);
		delete[]P;
	}

	int pcaFeatureNum;
	for (int i = 1; i < featureNum / step; i++) {
		pcaFeatureNum = i * step;
		double* train_featureData_PCA = new double[trainFf->classNum * trainFf->picNum * pcaFeatureNum];
		double* test_featureData_PCA = new double[testFf->classNum * testFf->picNum * pcaFeatureNum];
		PCA_reduct_dimension(trainFf->featureData, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, 
			trainFf->featureNum, pcaFeatureNum, train_featureData_PCA, test_featureData_PCA);
		P = new double [testFf->classNum * testFf->picNum * testFf->classNum];
		//Bayes������������ÿ������ͼ������20��ĸ���
		bayesClassify(train_featureData_PCA, test_featureData_PCA, trainFf->classNum, trainFf->picNum, testFf->picNum, pcaFeatureNum, P);//�����featureNumӦ���ǽ�ά��ά��

		//׼ȷ�ʼ���
		accuracy = calculate_accuracy(P, testFf->Label, testFf->classNum, testFf->picNum);
		cout << "��ȡ����ά��Ϊ" << pcaFeatureNum;
		printf("Ԥ��׼ȷ��Ϊ��%f\n", accuracy);

		delete[]train_featureData_PCA;
		delete[]test_featureData_PCA;
		delete[]P;
	}
}

// k-��ֵ����
void kmeans_img(cv::Mat& img, int C) {
	int i, j;
	int height, width;
	height = img.rows;
	width = img.cols;
	double* featureArray = new double[width * height];//ֻ����Ҷ�ͼ��
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			int t = img.at<cv::Vec3b>(i, j)[0];
			if (t != 0)
				t = t;
			featureArray[i * width + j] = img.at<cv::Vec3b>(i, j)[0] / 256.0;
		}
	}
	int* labelArray = new int[width * height];
	myKmeas(featureArray, labelArray, width * height, 1, C);//�Ҷ�ͼ��1��ͨ��
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			img.at<cv::Vec3b>(i, j)[0] = img.at<cv::Vec3b>(i, j)[1] = img.at<cv::Vec3b>(i, j)[2] = labelArray[i * width + j] * 256 / C;
		}
	}
	delete[]featureArray;
	delete[]labelArray;
}

int main()
{
	// ԭʼ������ȡ
	if (0) {
		int classNum = 20;//�����
		int trainNum = 16;//ѵ������ÿ�������ĸ���
		int testNum = 4;//���Լ���ÿ�������ĸ���
		int featureNum = 144;//�Ҷȹ�����������������16ά,Gabor������80ά

		//�����ȡ����ͼ���·��
		char fileroot[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Satellite Library/";//���Ǹ�·��

		//��ȡѵ��ͼ��������ÿ���ǵ�1��16��ͼѵ��
		char textureFeatureFile_train[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_HOG.txt";//�Ҷȹ������������ļ����

		//��ȡ����ͼ��������ÿ���ǵ�17��20��ͼ����
		char textureFeatureFile_test[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_HOG.txt";//�Ҷȹ������������ļ����

		group_extractHOGFeature(fileroot, classNum, 1, 16, textureFeatureFile_train);
		group_extractHOGFeature(fileroot, classNum, 17, 20, textureFeatureFile_test);
	}
	// �������
	if (0) {
		// �����ļ�·��
		std::vector<std::string> fileNames;
		fileNames.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_GLCM.txt");
		fileNames.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_Gabor.txt");

		// �����ļ��ϲ�������ļ�·��
		char fileOut[140] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_added.txt";

		int featureNums[3] = { 16, 80 }; // ���ļ�����������ά��
		int picNum = 4;               // ѵ���� (���Լ�) ��ÿ�������ĸ��� 
		int classNum = 20;				 // �����Ŀ

		FeatureFiles * Ff = new FeatureFiles;
		
		Ff = Ff->readFeatureFiles(fileNames, featureNums, picNum, classNum);
		Ff->outPutFile(fileOut);

		delete[]Ff;
	}
	// �����������з�������
	if (1) {
		std::vector<std::string> trainFileName;
		std::vector<std::string> testFileName;

		//trainFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_GLCM.txt");
		trainFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_Gabor.txt");
		// trainFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_COLOR.txt");
		trainFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_HOG.txt");

		//testFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_GLCM.txt");
		testFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_Gabor.txt");
		// testFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_COLOR.txt");
		testFileName.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_HOG.txt");

		int featureNums[4] = { 80, 144 };
		int trainPicNum = 16, testPicNum = 4;
		int classNum = 20;

		FeatureFiles* trainFf, * testFf;
		trainFf = new FeatureFiles;
		testFf = new FeatureFiles;
		trainFf = trainFf->readFeatureFiles(trainFileName, featureNums, trainPicNum, classNum);
		testFf = testFf->readFeatureFiles(testFileName, featureNums, testPicNum, classNum);

		normal(trainFf->featureData, trainFf->picNum * trainFf->classNum, trainFf->featureNum, testFf->featureData, testFf->picNum * testFf->classNum);

		// PCA������ά����
		if (1) {
			PCA_optimize(trainFf->featureNum, 4, trainFf, testFf);
		}

		// ���ǿ��޼ල����
		if (0) {
			int pcaFeatureNum = 2;
			double* train_featureData_PCA = new double[trainFf->classNum * trainFf->picNum * pcaFeatureNum];
			double* test_featureData_PCA = new double[testFf->classNum * testFf->picNum * pcaFeatureNum];
			// PCA������ά
			PCA_reduct_dimension(trainFf->featureData, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, trainFf->featureNum, pcaFeatureNum, train_featureData_PCA, test_featureData_PCA);
			// �޼ල��ѵ���������Լ����
			int i, j, k;
			double* featureData = new double[(trainFf->picNum + testFf->picNum) * trainFf->classNum * pcaFeatureNum];
			for (i = 0; i < classNum; i++) {
				for (j = 0; j < trainFf->picNum; j++) {//ѵ��ͼ��16��
					for (k = 0; k < pcaFeatureNum; k++)
						featureData[i * (trainFf->picNum + testFf->picNum) * pcaFeatureNum + j * pcaFeatureNum + k] = train_featureData_PCA[i * trainFf->picNum * pcaFeatureNum + j * pcaFeatureNum + k];
				}
				for (j = trainFf->picNum; j < (trainFf->picNum + testFf->picNum); j++) {//����ͼ��4��
					for (k = 0; k < pcaFeatureNum; k++)
						featureData[i * (trainFf->picNum + testFf->picNum) * pcaFeatureNum + j * pcaFeatureNum + k] = test_featureData_PCA[i * testFf->picNum * pcaFeatureNum + (j - trainFf->picNum) * pcaFeatureNum + k];
				}
			}
			int* labelArray = new int[400];
			int C = 20;//20������
			// k��ֵ�㷨
			if (0) {
				myKmeas(featureData, labelArray, 400, pcaFeatureNum, C);
				printf("׼ȷ��:%lf\n", accuracy(labelArray, classNum, 20));
				const char save_name[140] = "D:\\VS code\\projects\\mode recognition\\mode recognition experiment\\result\\clusterResult.txt";
				FILE* fp = fopen(save_name, "w");

				for (i = 0; i < 400; i++) {
					for (k = 0; k < pcaFeatureNum; k++) {
						fprintf(fp, "%10.2f", featureData[i * pcaFeatureNum + k]);
					}
					fprintf(fp, "%10d\n", labelArray[i] + 1);
				}
				fclose(fp);
			}

			// DBSCAN�㷨
			if (0) {
				double epsilon = 0.07;
				int minPts = 5;
				dbscan(labelArray, featureData, epsilon, minPts);
				cout << "׼ȷ��:" << accuracy(labelArray, 20, 20) << endl;
			}

			// ��ƽ���ּ�����
			if (0) {
				mean_distance(featureData, labelArray, C);
			}

			//�ͷ���Դ
			delete[]test_featureData_PCA;
			delete[]train_featureData_PCA;
			delete[]labelArray;
			delete[]featureData;
			delete testFf;
			delete trainFf;
		}

		//BP����
		if (0) {
			double *test_P = new double[testFf->classNum * testFf->picNum * testFf->classNum];
			double* train_P = new double[trainFf->classNum * trainFf->picNum * trainFf->classNum];
			BP_Classify(trainFf->featureData, trainFf->Label, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, trainFf->featureNum, test_P, train_P);
			//׼ȷ�ʼ���
			double test_accuracy = calculate_accuracy(test_P, testFf->Label, classNum, testFf->picNum);
			double train_accuracy = calculate_accuracy(train_P, trainFf->Label, classNum, trainFf->picNum);
			printf("���Լ���СΪ��%d\n", classNum * testFf->picNum);
			printf("Ԥ��׼ȷ��Ϊ��%f\n", test_accuracy);
			printf("ѵ������СΪ��%d\n", classNum* trainFf->picNum);
			printf("Ԥ��׼ȷ��Ϊ��%f\n", train_accuracy);
		}
	}
	// ���ŻҶ�ͼƬ���о���
	if (0) {
		string imgPath = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Satellite Library/s10/2.bmp";
		cv::Mat Img;
		Img = cv::imread(imgPath);//����ͼ��
		int C = 3;//�۳�C��
		kmeans_img(Img, C);//����k��ֵ����
		string save_name = "D:\\VS code\\projects\\mode recognition\\mode recognition experiment\\result\\outcome.bmp";
		cv::Mat img_gray;
		cv::cvtColor(Img, img_gray, cv::COLOR_BGR2GRAY);
		cv::imwrite(save_name, img_gray);
		cv::imshow("result", Img);
		cv::waitKey(0);
	}

	cout << "end";

}