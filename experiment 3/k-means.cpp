#include <stdio.h>
#include<math.h>
#include<memory.h>
#include<stdio.h>
#include<iostream>
using namespace std;
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
	//动态聚类
	while (1) {
		memcpy(oldM, class_i_m, featureNum*C*sizeof(double));//一共C个聚类中心，每个聚类中心包括featureNum分量
		memcpy(oldJe, je, C * sizeof(double));//每个类有一个误差平方和，一共C个类
		
		for (i = 0; i<sampleNum; i++) {
			for (k = 0; k<C; k++) {
				if (*(labelArray + i) == k) {//当前样本属于k类，将样本从k类中移出
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] - 1);
				}
				else {//将样本试探移入非k类
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum + j], 2);
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

				*(labelArray + i) = min;
			}

		}
		
		//最后的误差平方和变化小于5时终止迭代
		float det = 0;
		for (k = 0; k<C; k++)
			det += fabs(oldJe[k] - je[k]) + fabs(oldJe[k] - je[k]);
		if (det<5)
			break;
	}
	delete[]je;
	delete []oldM;
	delete []oldJe;
	delete []t;
}
//K均值聚类
//featureArray特征数组，labelArray聚类标号数组，sampleNum样本数，featureNum特征数，C个类别
//featureArray大小为sampleNum*featureNum，labelArray大小为sampleNum
void myKmeas(double *featureArray, int *labelArray, int sampleNum, int featureNum,int C)
{
	int i, j, k, l, min;
	double *class_i_m = new double[featureNum*C];
	int *class_i_Num = new int[C];
	
	//初始化样本标号;根据样本在数组中的下标，等间隔分配标号，记录每类样本个数
	memset(class_i_Num, 0, sizeof(int)*C);
	
	for (i = 0; i<sampleNum; i++) {
		labelArray[i] = i*C / sampleNum;//赋值标号，假设聚C类，标号为0,1,..,C-1
		class_i_Num[i*C / sampleNum]++;//标号为i*C / sampleNum的类计数
	}

	/*
	for (i = 0; i < sampleNum; i++) {
		labelArray[i] = i % C;
		class_i_Num[i % C]++;
	}
	*/
	
	//计算初始聚类中心
	memset(class_i_m, 0, sizeof(double)*featureNum*C);//清零
	for (i = 0; i<sampleNum; i++) {
		for (k = 0; k<C; k++) {
			if (*(labelArray + i) == k) {//当前样本是第k个类
				for(j=0;j<featureNum;j++)
					class_i_m[k*featureNum + j] += *(featureArray + i*featureNum + j);
			}
		}
	}

	for (k = 0; k < C; k++) {//每类样本聚类中心
		for (j = 0; j<featureNum; j++)
			class_i_m[k*featureNum + j] /= class_i_Num[k];
	}
	//迭代聚类
	Kmeans_clustering(featureArray, labelArray, sampleNum, featureNum,  class_i_m, class_i_Num, C);
	
	delete []class_i_m;
	delete []class_i_Num;
}

