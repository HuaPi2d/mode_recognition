#include"Normal_Bayes_Classifier.h"

#define feature 100 //最多不能超过这个维数，否则出错了
//计算样本特征均值
void calculate_mu(double traindata[], double Mu[][feature], int classNum, int trainNum, int featureNum)
{
	for (int c = 0; c < classNum; c++) {
		for (int i = 0; i < featureNum; i++) {
			double sum = 0.0;
			for (int j = 0; j < trainNum; j++) {
				sum += traindata[(c*trainNum + j)*featureNum + i];
			}
			Mu[c][i] = sum / trainNum;
		}
	}
}

//计算样本特征方差
void calculate_cov(double traindata[], double mu[][feature], double cov[][feature], int classNum, int trainNum, int featureNum)
{
	for (int c = 0; c < classNum; c++) {
		double sum = 0.0;
		for (int i = 0; i < featureNum; i++) {
			double sum = 0.0;
			for (int t = 0; t < trainNum; t++) {
				sum += (traindata[(c*trainNum + t)*featureNum + i] - mu[c][i])*(traindata[(c*trainNum + t)*featureNum + i] - mu[c][i]);
			}
			cov[c][i] = sum / trainNum;
		}
	}
}

//计算预测概率，假设特征属性独立，即不同特征间协方差为0
void calculate_p(double testdata[], double mu[][feature], double cov[][feature], double* P, int classNum, int testNum, int featureNum)
{
	double pi = 3.1415926;//圆周率
	for (int k = 0; k < classNum*testNum; k++) {
		for (int c = 0; c < classNum; c++) {
			double p = 1.0;
			for (int i = 0; i < featureNum; i++) {
				if (cov[c][i] < 1E-8) {
					if (fabs(mu[c][i] - testdata[k * featureNum + i]) < 1E-7) {
						p *= 1;
					}
					else {
						p *= 0;
					}
				}
				else {
					p *= sqrt(1.0 / (2.0 * pi * cov[c][i])) * exp(-pow((testdata[k * featureNum + i] - mu[c][i]), 2.0) / (2.0 * cov[c][i]));
				}
			}
			P[k * classNum + c] = p;
		}
	}
}


void bayesClassify(double traindata[], double testdata[], int classNum, int trainNum, int testNum, int featureNum,  double* P)
{
	//计算已知样本特征均值，给的固定大小，如果换数据集要记得调大小
	double mu[40][feature];
	calculate_mu(traindata, mu, classNum, trainNum, featureNum);

	//计算已知样本特征方差，给的固定大小，如果换数据集要记得调大小
	double cov[40][feature];
	calculate_cov(traindata, mu, cov, classNum, trainNum, featureNum);


	//对测试样本计算后验概率，用来做预测
	calculate_p(testdata, mu, cov, P, classNum, testNum, featureNum);
}

