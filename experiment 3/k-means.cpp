#pragma warning(disable:4996)
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include <stdio.h>
#include <iostream> 
#include <vector>  
#include <random>  
#include <cmath> 
#include "paintVsualize.h"
#include "calculate_intra_inter_packed.h"

#define DBL_MAX 10
using namespace std;
//k-means++

// �ṹ��ʾ��ά���ݵ�  
struct DataPoint {
	double x, y;
};

// ��������֮���ŷ�Ͼ����ƽ��  
double squaredDistance(const DataPoint& p1, const DataPoint& p2) {
	return std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
}

// �����K-means++ʵ�֣�����ֻ�ǿ�ܣ�  
std::vector<DataPoint> kmeansPlusPlus(const std::vector<DataPoint>& data, int k) {
	// ... ʵ��K-means++�㷨 ...  
	std::vector<DataPoint> centers(k);
	std::random_device rd;
	std::mt19937 gen(rd());

	// ��һ�������ѡ��һ������Ϊ��һ����������  
	std::uniform_int_distribution<> dis(0, data.size() - 1);
	int firstCenterIndex = dis(gen);
	centers[0] = data[firstCenterIndex];

	// �������裺ѡ��ʣ��ľ�������  
	double totalDist = 0.0;
	std::vector<double> probCumulativeDist(data.size(), 0.0);
	for (size_t i = 0; i < data.size(); ++i) {
		if (i == firstCenterIndex) continue;
		double dist = squaredDistance(data[i], centers[0]);
		totalDist += dist * dist; // ʹ�þ����ƽ����ΪȨ��  
		probCumulativeDist[i] = totalDist;
	}

	// ��һ�������ۻ��ֲ�  
	for (size_t i = 0; i < probCumulativeDist.size(); ++i) {
		probCumulativeDist[i] /= probCumulativeDist.back();
	}

	// Ϊʣ��ľ�������ѡ���  
	std::uniform_real_distribution<> disReal(0.0, 1.0);
	for (int i = 1; i < k; ++i) {
		double randNum = disReal(gen);
		for (size_t j = 0; j < data.size(); ++j) {
			if (randNum < probCumulativeDist[j]) {
				double minDist = std::numeric_limits<double>::max();
				DataPoint newCenter = data[j];
				for (const auto& center : centers) {
					double dist = squaredDistance(newCenter, center);
					if (dist < minDist) {
						minDist = dist;
					}
				}
				if (minDist > 0) { // ����ѡ���Ѵ��ڵľ�������  
					centers[i] = newCenter;
					break;
				}
			}
		}

		// ���¸����ۻ��ֲ�����ѡ���������Ӽ��㸴�Ӷȣ�  
		// ����Ϊ�˼򻯣�����ʡ������һ����ֻʹ�õ�һ�������������������  
	}

	return centers;
}

// K-means++ ��ʼ������  
void kmeansPlusPlusInit(const double* data, int numDataPoints, int k, int& numOutputPoints, double* outputCenters) {
	std::vector<DataPoint> dataPoints;
	// ��double*ת��ΪDataPoint����  
	for (int i = 0; i < numDataPoints; i++) {
		dataPoints.emplace_back(DataPoint{ data[2 * i], data[2 * i + 1] });
	}

	std::vector<DataPoint> centers = kmeansPlusPlus(dataPoints, k); // ��������ʵ����K-means++�ĺ���  

	// �������������ڴ�  
	numOutputPoints = centers.size();

	// ���������ĸ��Ƶ��������  
	for (int i = 0; i < numOutputPoints; i++) {
		outputCenters[2 * i] = centers[i].x;
		outputCenters[2 * i + 1] = centers[i].y;
	}
}

//K��ֵ�������
//featureArray��������, labelArray������飬sampleNum������, featureNum��������class_i_m��i��ľ������ģ�class_i_Num��i�����������C�����
//featureArray��sampleNum*featureNum��С��labelArray��sampleNum��С��class_i_m��featureNum*C��С
void Kmeans_clustering(double *featureArray, int labelArray[], int  sampleNum, int featureNum, double class_i_m[], int class_i_Num[], int C)
{
	int i, j, k, l;
	double *je= new double[C];//���ƽ����
	for (k = 0; k<C; k++)
		je[k] = 0;
	for (i = 0; i<sampleNum; i++) {//����������Ѿ��г�ʼ���ͳ�Ƴ�ʼ���ƽ����
		for (k = 0; k<C; k++) {
			if (*(labelArray + i) == k) {//��ǰ�������ڵ�k����
				for(j=0;j<featureNum;j++)
				   je[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum+j], 2);
			}
		}
	}

	int min, mark;
	double *oldM = new double[C*featureNum];
	double *oldJe=new double[C];
	double *t= new double[C];//��ʱ�������
	bool changed = false;

	//��̬����
	while (1) {
		changed = false;

		memcpy(oldM, class_i_m, featureNum*C*sizeof(double));//һ��C���������ģ�ÿ���������İ���featureNum����
		memcpy(oldJe, je, C * sizeof(double));//ÿ������һ�����ƽ���ͣ�һ��C����
		
		for (i = 0; i<sampleNum; i++) {
			if (class_i_Num[labelArray[i]] == 0 || class_i_Num[labelArray[i]] == 1) {
				continue;
			}
			for (k = 0; k<C; k++) {
				if (labelArray [i] == k) {//��ǰ��������k�࣬��������k�����Ƴ�
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(featureArray[i * featureNum + j] - class_i_m[k * featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] - 1);
				}
				else {   //��������̽�����k��
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(featureArray[i * featureNum + j] - class_i_m[k*featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] + 1);
				}
			}
			min = 0;//����С�����ƽ����
			for (k = 1; k<C; k++) {
				if (t[k]<t[min])
					min = k;
			}
			if (min != *(labelArray + i)) {//�����������µ��࣬���¾������ĺ����ƽ����
				mark = *(labelArray + i);
				for(j=0;j<featureNum;j++)
					class_i_m[mark*featureNum+j] += (class_i_m[mark*featureNum + j] - *(featureArray + i*featureNum + j)) / (class_i_Num[mark] - 1);
				class_i_Num[mark]--;
				je[mark] -= t[mark];

				for (j = 0; j<featureNum; j++)
					class_i_m[min*featureNum + j] += (*(featureArray + i*featureNum + j)-class_i_m[min*featureNum + j] ) / (class_i_Num[min] + 1);
				class_i_Num[min]++;
				je[min] += t[min];

				if (labelArray[i] != min) {
					labelArray[i] = min;
					changed = true;
				}
			}
		}
		//�������ƽ���ͱ仯С��5ʱ��ֹ����
		float det = 0;
		for (k = 0; k<C; k++)
			det += fabs(oldJe[k] - je[k]) + fabs(oldJe[k] - je[k]);
		if (changed == false) {
			break;
		}
		// paint_2D_silhouette(featureArray, labelArray, class_i_m, "result");
		// justify(featureArray, labelArray);
		// if (det < 0.05)
			//break;
	}
	delete[]je;
	delete []oldM;
	delete []oldJe;
	delete []t;
}

void Kmeans_nearest(double* featureArray, int labelArray[], int sampleNum, int featureNum, double class_i_m[], int class_i_Num[], int C) {
	double* oldM = new double[featureNum * C];
	double* distance = new double[C];
	bool changed = true; // �����ж��Ƿ���������Ѹı�  
	double min;

	while (changed) {
		changed = false;
		// ��¼ԭʼ��������  
		memcpy(oldM, class_i_m, featureNum * C * sizeof(double));

		// ��ʼ�����������ۼ�����������  
		for (int i = 0; i < C; i++) {
			class_i_Num[i] = 0;
			for (int j = 0; j < featureNum; j++) {
				class_i_m[i * featureNum + j] = 0;
			}
		}

		// ���������ĸ������µľ�������  
		for (int i = 0; i < sampleNum; i++) {
			int curr_class = labelArray[i];
			class_i_Num[curr_class]++;
			for (int j = 0; j < featureNum; j++) {
				class_i_m[curr_class * featureNum + j] += featureArray[i * featureNum + j];
			}
		}

		// ����ƽ����������  
		for (int i = 0; i < C; i++) {
			if (class_i_Num[i] > 0) { // �������0  
				for (int j = 0; j < featureNum; j++) {
					class_i_m[i * featureNum + j] /= class_i_Num[i];
				}
			}
		}

		// ���¹���  
		for (int i = 0; i < sampleNum; i++) {
			min = DBL_MAX; // ʹ��DBL_MAX��Ϊ��ʼ���ֵ  
			int nearest_class = -1;
			for (int j = 0; j < C; j++) {
				double dist = 0;
				for (int k = 0; k < featureNum; k++) {
					dist += pow(featureArray[i * featureNum + k] - class_i_m[j * featureNum + k], 2);
				}

				distance[j] = sqrt(dist);

				if (distance[j] < min) {
					min = distance[j];
					nearest_class = j;
				}
			}
			if (nearest_class != labelArray[i]) { // ������ı���  
				labelArray[i] = nearest_class;
				changed = true; // ��Ǿ��������Ѹı�  
			}
		}

		if (changed == false) {
			break;
		}
	}

	delete[]oldM;
	delete[]distance;
}

//K��ֵ����
//featureArray�������飬labelArray���������飬sampleNum��������featureNum��������C�����
//featureArray��СΪsampleNum*featureNum��labelArray��СΪsampleNum
void myKmeas(double *featureArray, int *labelArray, int sampleNum, int featureNum,int C)
{
	int i, j, k, l, min;
	double *class_i_m = new double[featureNum*C];
	int *class_i_Num = new int[C];

	memset(class_i_Num, 0, sizeof(int) * C);

	// ȷ���������ģ��ȼ�������ǩȷ����������
	if (0) 
	{
		memset(class_i_m, 0, sizeof(double) * featureNum * C);

		for (i = 0; i < sampleNum; i++) {
			labelArray[i] = i * C / sampleNum;//��ֵ��ţ������C�࣬���Ϊ0,1,..,C-1
			class_i_Num[i * C / sampleNum]++;//���Ϊi*C / sampleNum�������
		}
		for (i = 0; i<sampleNum; i++) {
			for (k = 0; k<C; k++) {
				if (labelArray [i] == k) {//��ǰ�����ǵ�k����
					for(j=0;j<featureNum;j++)
						class_i_m[k * featureNum + j] += featureArray[i * featureNum + j];
				}
			}
		}
		for (k = 0; k < C; k++) {//ÿ��������������
			for (j = 0; j < featureNum; j++)
				class_i_m[k * featureNum + j] /= class_i_Num[k];
		}
	}
	// ȷ���������ģ��ȼ�������ǩȷ����������
	if (1)
	{
		memset(class_i_m, 0, sizeof(double) * featureNum * C);

		for (i = 0; i < sampleNum; i++) {
			labelArray[i] = i / C;//��ֵ��ţ������C�࣬���Ϊ0,1,..,C-1
			class_i_Num[i / C]++;//���Ϊi*C / sampleNum�������
		}
		for (i = 0; i < sampleNum; i++) {
			for (k = 0; k < C; k++) {
				if (labelArray[i] == k) {//��ǰ�����ǵ�k����
					for (j = 0; j < featureNum; j++)
						class_i_m[k * featureNum + j] += featureArray[i * featureNum + j];
				}
			}
		}
		for (k = 0; k < C; k++) {//ÿ��������������
			for (j = 0; j < featureNum; j++)
				class_i_m[k * featureNum + j] /= class_i_Num[k];
		}
	}
	// K-means++ �Ż���������ѡ�񷽷�
	if (0) 
	{
		int k;

		//��ʼ���������;��������
		memset(class_i_m, 0, sizeof(double) * featureNum * C);

		kmeansPlusPlusInit(featureArray, sampleNum, C, k, class_i_m);
		
		double min = 1000;
		int a = 0;
		DataPoint* m = new DataPoint;
		DataPoint* s = new DataPoint;
		for (i = 0; i < sampleNum; i++) {
			s->x = featureArray[i * 2];
			s->y = featureArray[i * 2 + 1];
			for (j = 0; j < C; j++) {
				m->x = class_i_m[j * 2];
				m->y = class_i_m[j * 2 + 1];

				if (squaredDistance(*m, *s) < min) {
					a = j;
					min = squaredDistance(*m, *s);
				}
			}
			class_i_Num[a]++;
			labelArray[i] = a;
			min = 1000;
		}
	}
	// ��ʼ���
	paint_2D_silhouette(featureArray, labelArray, class_i_m, "original");

	// ��������
	Kmeans_nearest(featureArray, labelArray, sampleNum, featureNum,  class_i_m, class_i_Num, C);
	// Kmeans_clustering(featureArray, labelArray, sampleNum, featureNum, class_i_m, class_i_Num, C);
	cout << "�������" << endl;

	// ������
	paint_2D_silhouette(featureArray, labelArray, class_i_m, "result");
	
	delete []class_i_m;
	delete []class_i_Num;
}

