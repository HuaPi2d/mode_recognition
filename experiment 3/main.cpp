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


//最大最小归一化，每个特征一列进行归一化，测试样本按训练样本的最大最小归一化
void normal(double traindata[], int trainSampleNum, int featureNum, double testdata[], int testSampleNum)
{
	double *train_featureData_max_min=new double[featureNum*2];
	for (int i = 0; i < featureNum; i++) {//找到每维特征的最大值和最小值
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
	//将训练样本归一化，线性映射到0-1之间
	for (int j = 0; j < featureNum; j++){
		for (int i = 0; i < trainSampleNum; i++) 
			if (fabs(train_featureData_max_min[j * 2 + 0] - train_featureData_max_min[j * 2 + 1]) < 1E-7) {
				traindata[i * featureNum + j] = 0;
			}
			else {
				traindata[i*featureNum +j] =(traindata[i*featureNum +j] - train_featureData_max_min[j*2+1]) / (train_featureData_max_min[j*2+0] - train_featureData_max_min[j*2+1]);
			}
	}
	
	//将测试样本归一化，按训练样本的最大最小值处理，有可能不在0-1之间
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

// 非线性归一化
void abnormal(double traindata[], int trainSampleNum, int featureNum, double testdata[], int testSampleNum) {
	double* mu = new double[featureNum];
	double sumj;
	double* sigma = new double[featureNum];

	// 求第j维特征的均值
	for (int j = 0; j < featureNum; j++) {
		sumj = 0;
		for (int i = 0; i < trainSampleNum; i++) {
			sumj += traindata[i * featureNum + j];
		}
		mu[j] = sumj / trainSampleNum;
	}

	// 求第j维特征的标准差
	for (int j = 0; j < featureNum; j++) {
		sumj = 0;
		for (int i = 0; i < trainSampleNum; i++) {
			sumj += (traindata[i * featureNum + j] - mu[j]) * (traindata[i * featureNum + j] - mu[j]);
		}
		sigma[j] = sqrt(sumj / trainSampleNum);
	}

	// 测试样本归一化
	for (int j = 0; j < featureNum; j++) {
		for (int i = 0; i < trainSampleNum; i++) {
			traindata[i * featureNum + j] = ((traindata[i * featureNum + j]) / (3 * sigma[j]) + 1) / 2; //非线性归一化公式
		}
	}

	// 训练样本归一化
	for (int j = 0; j < featureNum; j++) {
		for (int i = 0; i < testSampleNum; i++) {
			testdata[i * featureNum + j] = ((testdata[i * featureNum + j]) / (3 * sigma[j]) + 1) / 2;
		}
	}
}

//trainDataIn为训练特征，testDataIn为测试特征，pcaFeatureNum为降维后特征维数，trainDataOut为降维后训练特征, testDataOut为降维后测试特征
void PCA_reduct_dimension(double trainDataIn[], double testDataIn[], int classNum, int trainNum, int testNum, int featureNum, int pcaFeatureNum, double trainDataOut[], double testDataOut[])
{
	//把读入的特征转化成cv::Mat类型，第一维是样本数，第二维是特征数
	// cout << classNum << ' ' << trainNum << ' ' << featureNum << endl;
	cv::Mat trainDataIn_Mat(classNum*trainNum, featureNum, CV_64FC1);
	for (int i = 0; i < classNum*trainNum; i++) {
		for (int j = 0; j < featureNum; j++) {
			trainDataIn_Mat.at<double>(i, j) = trainDataIn[i*featureNum + j];//把数据复制到trainDataIn_cv::Mat
		}
	}
	//PCA分解得到输入特征向量。
	//trainDataIn_cv::Mat输入要降维的数据，cv::Mat()是个空数据，则系统自动从输入数据计算均值
	//CV_PCA_DATA_AS_ROW，标志量，输入数据一行是一个样本，pcaFeatureNum降维后保留的维数，最多和输入维数相同
	cv::PCA pca(trainDataIn_Mat, cv::Mat(), cv::PCA::DATA_AS_ROW, pcaFeatureNum);//cv::PCA::DATA_AS_ROW  CV_PCA_DATA_AS_ROW

	//对特征向量pca.eigenvectors转置
	transpose(pca.eigenvectors, pca.eigenvectors);
	//计算降维后特征数据
	cv::Mat trainDataOut_Mat = trainDataIn_Mat * pca.eigenvectors;//矩阵相乘
	//cout << pca.eigenvectors.cols << endl;

	//对测试数据降维，但用训练集的特征向量矩阵，假定的是测试数据和训练数据服从相同分布
	//把读入的特征转化成Mat类型，第一维是样本数，第二维是特征数
	cv::Mat testDataIn_Mat(classNum*testNum, featureNum, CV_64FC1);
	for (int i = 0; i < classNum*testNum; i++) {
		for (int j = 0; j < featureNum; j++) {
			testDataIn_Mat.at<double>(i, j) = testDataIn[i*featureNum + j];
		}
	}

	cv::Mat testDataOut_Mat = testDataIn_Mat * pca.eigenvectors;//测试数据降维后的特征
	//cout << testDataOut_cv::Mat.cols << endl;

	//将训练集和测试集PCA降维后数据转为double数组类型
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

//统计准确率
double calculate_accuracy(double* P, int test_Label[], int classNum, int testNum)
{
	double accuracy;
	int correct_P = 0;//正确预测的个数
	for (int k = 0; k < classNum * testNum; k++) {
		double tmp_max_p;
		int prediction_L = 1;//预测类别
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
			// cout << "错分样本：\n" << "类：" << k / testNum + 1 << " 样本序号：" << k % testNum + 17 << " 错分为类：" << prediction_L << endl;
		}
	}
	accuracy = double(correct_P) / (classNum * testNum); //计算得预测准确率
	return accuracy;
}

// cluster accuracy 聚类准确率
double accuracy(int* Labelarray, int classNum, int picNum) {
	int* class_Num = new int[classNum * 2];
	int right_Num = 0;
	int max;
	for (int i = 0; i < classNum; i++) {
		// 初始化
		for (int j = 0; j < classNum * 2; j++) {
			class_Num[j] = 0;
		}
		max = 0;

		// 计算一组样本中最大类别数目
		for (int k = 0; k < picNum; k++) {
			if (Labelarray[i * classNum + k] == -2) {
				continue;
			}
			class_Num[Labelarray[i * classNum + k]]++;
			if (class_Num[Labelarray[i * classNum + k]] > max) {
				max = class_Num[Labelarray[i * classNum + k]];
			}
		}

		// 计算每组数据中的正确预测的数目
		cout << "类别" << i << ':'  << max << endl;
		right_Num += max;
	}
	return (right_Num * 1.0) / (classNum * picNum);
}

// PCA最优化
void PCA_optimize(int featureNum, int step, FeatureFiles* trainFf, FeatureFiles* testFf) 
{
	double* P; //用来存放预测概率
	double accuracy; // 准确率

	// 原始特征不降维
	if (featureNum > 80) {
		featureNum = 80;
	}
	else {
		P = new double[testFf->classNum * testFf->picNum * testFf->classNum];
		//bayesClassify(train_featureData, test_featureData, classNum, trainNum, testNum, featureNum, P);
		bayesClassify(trainFf->featureData, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, featureNum, P);
		//cout_2D_array(P, testFf->classNum * testFf->picNum, testFf->classNum);
		//准确率计算
		accuracy = calculate_accuracy(P, testFf->Label, testFf->classNum, testFf->picNum);
		// printf("测试集大小为：%d\n", classNum*testNum);
		cout << "原始特征特征维数为" << featureNum;
		printf("预测准确率为：%f\n", accuracy);
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
		//Bayes分类器，计算每个测试图像属于20类的概率
		bayesClassify(train_featureData_PCA, test_featureData_PCA, trainFf->classNum, trainFf->picNum, testFf->picNum, pcaFeatureNum, P);//这里的featureNum应该是降维后维数

		//准确率计算
		accuracy = calculate_accuracy(P, testFf->Label, testFf->classNum, testFf->picNum);
		cout << "提取特征维数为" << pcaFeatureNum;
		printf("预测准确率为：%f\n", accuracy);

		delete[]train_featureData_PCA;
		delete[]test_featureData_PCA;
		delete[]P;
	}
}

// k-均值聚类
void kmeans_img(cv::Mat& img, int C) {
	int i, j;
	int height, width;
	height = img.rows;
	width = img.cols;
	double* featureArray = new double[width * height];//只处理灰度图像
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			int t = img.at<cv::Vec3b>(i, j)[0];
			if (t != 0)
				t = t;
			featureArray[i * width + j] = img.at<cv::Vec3b>(i, j)[0] / 256.0;
		}
	}
	int* labelArray = new int[width * height];
	myKmeas(featureArray, labelArray, width * height, 1, C);//灰度图像1个通道
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
	// 原始特征获取
	if (0) {
		int classNum = 20;//类别数
		int trainNum = 16;//训练集中每类样本的个数
		int testNum = 4;//测试集中每类样本的个数
		int featureNum = 144;//灰度共生矩阵纹理特征数16维,Gabor特征数80维

		//定义读取数据图像根路径
		char fileroot[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Satellite Library/";//卫星根路径

		//提取训练图像特征，每颗星第1至16张图训练
		char textureFeatureFile_train[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/trainset_HOG.txt";//灰度共生矩阵特征文件输出

		//提取测试图像特征，每颗星第17至20张图测试
		char textureFeatureFile_test[120] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_HOG.txt";//灰度共生矩阵特征文件输出

		group_extractHOGFeature(fileroot, classNum, 1, 16, textureFeatureFile_train);
		group_extractHOGFeature(fileroot, classNum, 17, 20, textureFeatureFile_test);
	}
	// 特征组合
	if (0) {
		// 输入文件路径
		std::vector<std::string> fileNames;
		fileNames.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_GLCM.txt");
		fileNames.push_back("D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_Gabor.txt");

		// 特征文件合并后输出文件路径
		char fileOut[140] = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Experiment Data/testset_added.txt";

		int featureNums[3] = { 16, 80 }; // 各文件包含的特征维数
		int picNum = 4;               // 训练集 (测试集) 中每类样本的个数 
		int classNum = 20;				 // 类别数目

		FeatureFiles * Ff = new FeatureFiles;
		
		Ff = Ff->readFeatureFiles(fileNames, featureNums, picNum, classNum);
		Ff->outPutFile(fileOut);

		delete[]Ff;
	}
	// 利用特征进行分类任务
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

		// PCA特征降维测试
		if (1) {
			PCA_optimize(trainFf->featureNum, 4, trainFf, testFf);
		}

		// 卫星库无监督分类
		if (0) {
			int pcaFeatureNum = 2;
			double* train_featureData_PCA = new double[trainFf->classNum * trainFf->picNum * pcaFeatureNum];
			double* test_featureData_PCA = new double[testFf->classNum * testFf->picNum * pcaFeatureNum];
			// PCA降成两维
			PCA_reduct_dimension(trainFf->featureData, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, trainFf->featureNum, pcaFeatureNum, train_featureData_PCA, test_featureData_PCA);
			// 无监督将训练集，测试集组合
			int i, j, k;
			double* featureData = new double[(trainFf->picNum + testFf->picNum) * trainFf->classNum * pcaFeatureNum];
			for (i = 0; i < classNum; i++) {
				for (j = 0; j < trainFf->picNum; j++) {//训练图像16张
					for (k = 0; k < pcaFeatureNum; k++)
						featureData[i * (trainFf->picNum + testFf->picNum) * pcaFeatureNum + j * pcaFeatureNum + k] = train_featureData_PCA[i * trainFf->picNum * pcaFeatureNum + j * pcaFeatureNum + k];
				}
				for (j = trainFf->picNum; j < (trainFf->picNum + testFf->picNum); j++) {//测试图像4张
					for (k = 0; k < pcaFeatureNum; k++)
						featureData[i * (trainFf->picNum + testFf->picNum) * pcaFeatureNum + j * pcaFeatureNum + k] = test_featureData_PCA[i * testFf->picNum * pcaFeatureNum + (j - trainFf->picNum) * pcaFeatureNum + k];
				}
			}
			int* labelArray = new int[400];
			int C = 20;//20颗卫星
			// k均值算法
			if (0) {
				myKmeas(featureData, labelArray, 400, pcaFeatureNum, C);
				printf("准确率:%lf\n", accuracy(labelArray, classNum, 20));
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

			// DBSCAN算法
			if (0) {
				double epsilon = 0.07;
				int minPts = 5;
				dbscan(labelArray, featureData, epsilon, minPts);
				cout << "准确率:" << accuracy(labelArray, 20, 20) << endl;
			}

			// 组平均分级聚类
			if (0) {
				mean_distance(featureData, labelArray, C);
			}

			//释放资源
			delete[]test_featureData_PCA;
			delete[]train_featureData_PCA;
			delete[]labelArray;
			delete[]featureData;
			delete testFf;
			delete trainFf;
		}

		//BP网络
		if (0) {
			double *test_P = new double[testFf->classNum * testFf->picNum * testFf->classNum];
			double* train_P = new double[trainFf->classNum * trainFf->picNum * trainFf->classNum];
			BP_Classify(trainFf->featureData, trainFf->Label, testFf->featureData, trainFf->classNum, trainFf->picNum, testFf->picNum, trainFf->featureNum, test_P, train_P);
			//准确率计算
			double test_accuracy = calculate_accuracy(test_P, testFf->Label, classNum, testFf->picNum);
			double train_accuracy = calculate_accuracy(train_P, trainFf->Label, classNum, trainFf->picNum);
			printf("测试集大小为：%d\n", classNum * testFf->picNum);
			printf("预测准确率为：%f\n", test_accuracy);
			printf("训练集大小为：%d\n", classNum* trainFf->picNum);
			printf("预测准确率为：%f\n", train_accuracy);
		}
	}
	// 单张灰度图片进行聚类
	if (0) {
		string imgPath = "D:/VS code/projects/mode recognition/mode recognition experiment/experiment 1/Satellite Library/s10/2.bmp";
		cv::Mat Img;
		Img = cv::imread(imgPath);//读入图像
		int C = 3;//聚成C类
		kmeans_img(Img, C);//进行k均值聚类
		string save_name = "D:\\VS code\\projects\\mode recognition\\mode recognition experiment\\result\\outcome.bmp";
		cv::Mat img_gray;
		cv::cvtColor(Img, img_gray, cv::COLOR_BGR2GRAY);
		cv::imwrite(save_name, img_gray);
		cv::imshow("result", Img);
		cv::waitKey(0);
	}

	cout << "end";

}