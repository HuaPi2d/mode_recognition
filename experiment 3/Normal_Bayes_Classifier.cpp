#include"Normal_Bayes_Classifier.h"

#define feature 100 //��಻�ܳ������ά�������������
//��������������ֵ
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

//����������������
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

//����Ԥ����ʣ������������Զ���������ͬ������Э����Ϊ0
void calculate_p(double testdata[], double mu[][feature], double cov[][feature], double* P, int classNum, int testNum, int featureNum)
{
	double pi = 3.1415926;//Բ����
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
	//������֪����������ֵ�����Ĺ̶���С����������ݼ�Ҫ�ǵõ���С
	double mu[40][feature];
	calculate_mu(traindata, mu, classNum, trainNum, featureNum);

	//������֪��������������Ĺ̶���С����������ݼ�Ҫ�ǵõ���С
	double cov[40][feature];
	calculate_cov(traindata, mu, cov, classNum, trainNum, featureNum);


	//�Բ����������������ʣ�������Ԥ��
	calculate_p(testdata, mu, cov, P, classNum, testNum, featureNum);
}

