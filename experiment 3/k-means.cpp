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

// 结构表示二维数据点  
struct DataPoint {
	double x, y;
};

// 计算两点之间的欧氏距离的平方  
double squaredDistance(const DataPoint& p1, const DataPoint& p2) {
	return std::pow(p1.x - p2.x, 2) + std::pow(p1.y - p2.y, 2);
}

// 假设的K-means++实现（这里只是框架）  
std::vector<DataPoint> kmeansPlusPlus(const std::vector<DataPoint>& data, int k) {
	// ... 实现K-means++算法 ...  
	std::vector<DataPoint> centers(k);
	std::random_device rd;
	std::mt19937 gen(rd());

	// 第一步：随机选择一个点作为第一个聚类中心  
	std::uniform_int_distribution<> dis(0, data.size() - 1);
	int firstCenterIndex = dis(gen);
	centers[0] = data[firstCenterIndex];

	// 后续步骤：选择剩余的聚类中心  
	double totalDist = 0.0;
	std::vector<double> probCumulativeDist(data.size(), 0.0);
	for (size_t i = 0; i < data.size(); ++i) {
		if (i == firstCenterIndex) continue;
		double dist = squaredDistance(data[i], centers[0]);
		totalDist += dist * dist; // 使用距离的平方作为权重  
		probCumulativeDist[i] = totalDist;
	}

	// 归一化概率累积分布  
	for (size_t i = 0; i < probCumulativeDist.size(); ++i) {
		probCumulativeDist[i] /= probCumulativeDist.back();
	}

	// 为剩余的聚类中心选择点  
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
				if (minDist > 0) { // 避免选择已存在的聚类中心  
					centers[i] = newCenter;
					break;
				}
			}
		}

		// 更新概率累积分布（可选，但会增加计算复杂度）  
		// 这里为了简化，我们省略了这一步，只使用第一个聚类中心来计算概率  
	}

	return centers;
}

// K-means++ 初始化函数  
void kmeansPlusPlusInit(const double* data, int numDataPoints, int k, int& numOutputPoints, double* outputCenters) {
	std::vector<DataPoint> dataPoints;
	// 将double*转换为DataPoint向量  
	for (int i = 0; i < numDataPoints; i++) {
		dataPoints.emplace_back(DataPoint{ data[2 * i], data[2 * i + 1] });
	}

	std::vector<DataPoint> centers = kmeansPlusPlus(dataPoints, k); // 假设这是实现了K-means++的函数  

	// 分配输出数组的内存  
	numOutputPoints = centers.size();

	// 将聚类中心复制到输出数组  
	for (int i = 0; i < numOutputPoints; i++) {
		outputCenters[2 * i] = centers[i].x;
		outputCenters[2 * i + 1] = centers[i].y;
	}
}

//K均值聚类迭代
//featureArray特征数组, labelArray标号数组，sampleNum样本数, featureNum特征数，class_i_m第i类的聚类中心，class_i_Num第i类的样本数，C个类别
//featureArray是sampleNum*featureNum大小，labelArray是sampleNum大小，class_i_m是featureNum*C大小
void Kmeans_clustering(double *featureArray, int labelArray[], int  sampleNum, int featureNum, double class_i_m[], int class_i_Num[], int C)
{
	int i, j, k, l;
	double *je= new double[C];//误差平方和
	for (k = 0; k<C; k++)
		je[k] = 0;
	for (i = 0; i<sampleNum; i++) {//输入的样本已经有初始类别，统计初始误差平方和
		for (k = 0; k<C; k++) {
			if (*(labelArray + i) == k) {//当前样本属于第k个类
				for(j=0;j<featureNum;j++)
				   je[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum+j], 2);
			}
		}
	}

	int min, mark;
	double *oldM = new double[C*featureNum];
	double *oldJe=new double[C];
	double *t= new double[C];//临时计算误差
	bool changed = false;

	//动态聚类
	while (1) {
		changed = false;

		memcpy(oldM, class_i_m, featureNum*C*sizeof(double));//一共C个聚类中心，每个聚类中心包括featureNum分量
		memcpy(oldJe, je, C * sizeof(double));//每个类有一个误差平方和，一共C个类
		
		for (i = 0; i<sampleNum; i++) {
			if (class_i_Num[labelArray[i]] == 0 || class_i_Num[labelArray[i]] == 1) {
				continue;
			}
			for (k = 0; k<C; k++) {
				if (labelArray [i] == k) {//当前样本属于k类，将样本从k类中移出
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(featureArray[i * featureNum + j] - class_i_m[k * featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] - 1);
				}
				else {   //将样本试探移入非k类
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(featureArray[i * featureNum + j] - class_i_m[k*featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] + 1);
				}
			}
			min = 0;//找最小的误差平方和
			for (k = 1; k<C; k++) {
				if (t[k]<t[min])
					min = k;
			}
			if (min != *(labelArray + i)) {//将样本移入新的类，更新聚类中心和误差平方和
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
		//最后的误差平方和变化小于5时终止迭代
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
	bool changed = true; // 用于判断是否聚类中心已改变  
	double min;

	while (changed) {
		changed = false;
		// 记录原始聚类中心  
		memcpy(oldM, class_i_m, featureNum * C * sizeof(double));

		// 初始化聚类中心累加器和样本数  
		for (int i = 0; i < C; i++) {
			class_i_Num[i] = 0;
			for (int j = 0; j < featureNum; j++) {
				class_i_m[i * featureNum + j] = 0;
			}
		}

		// 计算各类别点的个数和新的聚类中心  
		for (int i = 0; i < sampleNum; i++) {
			int curr_class = labelArray[i];
			class_i_Num[curr_class]++;
			for (int j = 0; j < featureNum; j++) {
				class_i_m[curr_class * featureNum + j] += featureArray[i * featureNum + j];
			}
		}

		// 计算平均聚类中心  
		for (int i = 0; i < C; i++) {
			if (class_i_Num[i] > 0) { // 避免除以0  
				for (int j = 0; j < featureNum; j++) {
					class_i_m[i * featureNum + j] /= class_i_Num[i];
				}
			}
		}

		// 重新归类  
		for (int i = 0; i < sampleNum; i++) {
			min = DBL_MAX; // 使用DBL_MAX作为初始最大值  
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
			if (nearest_class != labelArray[i]) { // 如果类别改变了  
				labelArray[i] = nearest_class;
				changed = true; // 标记聚类中心已改变  
			}
		}

		if (changed == false) {
			break;
		}
	}

	delete[]oldM;
	delete[]distance;
}

//K均值聚类
//featureArray特征数组，labelArray聚类标号数组，sampleNum样本数，featureNum特征数，C个类别
//featureArray大小为sampleNum*featureNum，labelArray大小为sampleNum
void myKmeas(double *featureArray, int *labelArray, int sampleNum, int featureNum,int C)
{
	int i, j, k, l, min;
	double *class_i_m = new double[featureNum*C];
	int *class_i_Num = new int[C];

	memset(class_i_Num, 0, sizeof(int) * C);

	// 确定聚类中心，等间隔分配标签确定聚类中心
	if (0) 
	{
		memset(class_i_m, 0, sizeof(double) * featureNum * C);

		for (i = 0; i < sampleNum; i++) {
			labelArray[i] = i * C / sampleNum;//赋值标号，假设聚C类，标号为0,1,..,C-1
			class_i_Num[i * C / sampleNum]++;//标号为i*C / sampleNum的类计数
		}
		for (i = 0; i<sampleNum; i++) {
			for (k = 0; k<C; k++) {
				if (labelArray [i] == k) {//当前样本是第k个类
					for(j=0;j<featureNum;j++)
						class_i_m[k * featureNum + j] += featureArray[i * featureNum + j];
				}
			}
		}
		for (k = 0; k < C; k++) {//每类样本聚类中心
			for (j = 0; j < featureNum; j++)
				class_i_m[k * featureNum + j] /= class_i_Num[k];
		}
	}
	// 确定聚类中心，等间隔分配标签确定聚类中心
	if (1)
	{
		memset(class_i_m, 0, sizeof(double) * featureNum * C);

		for (i = 0; i < sampleNum; i++) {
			labelArray[i] = i / C;//赋值标号，假设聚C类，标号为0,1,..,C-1
			class_i_Num[i / C]++;//标号为i*C / sampleNum的类计数
		}
		for (i = 0; i < sampleNum; i++) {
			for (k = 0; k < C; k++) {
				if (labelArray[i] == k) {//当前样本是第k个类
					for (j = 0; j < featureNum; j++)
						class_i_m[k * featureNum + j] += featureArray[i * featureNum + j];
				}
			}
		}
		for (k = 0; k < C; k++) {//每类样本聚类中心
			for (j = 0; j < featureNum; j++)
				class_i_m[k * featureNum + j] /= class_i_Num[k];
		}
	}
	// K-means++ 优化聚类中心选择方法
	if (0) 
	{
		int k;

		//初始化样本标号;聚类中心
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
	// 初始情况
	paint_2D_silhouette(featureArray, labelArray, class_i_m, "original");

	// 迭代聚类
	Kmeans_nearest(featureArray, labelArray, sampleNum, featureNum,  class_i_m, class_i_Num, C);
	// Kmeans_clustering(featureArray, labelArray, sampleNum, featureNum, class_i_m, class_i_Num, C);
	cout << "聚类完成" << endl;

	// 聚类结果
	paint_2D_silhouette(featureArray, labelArray, class_i_m, "result");
	
	delete []class_i_m;
	delete []class_i_Num;
}

