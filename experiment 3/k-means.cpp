#include <stdio.h>
#include<math.h>
#include<memory.h>
#include<stdio.h>
#include<iostream>
using namespace std;
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
	//��̬����
	while (1) {
		memcpy(oldM, class_i_m, featureNum*C*sizeof(double));//һ��C���������ģ�ÿ���������İ���featureNum����
		memcpy(oldJe, je, C * sizeof(double));//ÿ������һ�����ƽ���ͣ�һ��C����
		
		for (i = 0; i<sampleNum; i++) {
			for (k = 0; k<C; k++) {
				if (*(labelArray + i) == k) {//��ǰ��������k�࣬��������k�����Ƴ�
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum + j], 2);
					t[k] = class_i_Num[k] * t[k] / (class_i_Num[k] - 1);
				}
				else {//��������̽�����k��
					t[k] = 0;
					for (j = 0; j<featureNum; j++)
						t[k] += pow(*(featureArray + i*featureNum + j) - class_i_m[k*featureNum + j], 2);
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

				*(labelArray + i) = min;
			}

		}
		
		//�������ƽ���ͱ仯С��5ʱ��ֹ����
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
//K��ֵ����
//featureArray�������飬labelArray���������飬sampleNum��������featureNum��������C�����
//featureArray��СΪsampleNum*featureNum��labelArray��СΪsampleNum
void myKmeas(double *featureArray, int *labelArray, int sampleNum, int featureNum,int C)
{
	int i, j, k, l, min;
	double *class_i_m = new double[featureNum*C];
	int *class_i_Num = new int[C];
	
	//��ʼ���������;���������������е��±꣬�ȼ�������ţ���¼ÿ����������
	memset(class_i_Num, 0, sizeof(int)*C);
	
	for (i = 0; i<sampleNum; i++) {
		labelArray[i] = i*C / sampleNum;//��ֵ��ţ������C�࣬���Ϊ0,1,..,C-1
		class_i_Num[i*C / sampleNum]++;//���Ϊi*C / sampleNum�������
	}

	/*
	for (i = 0; i < sampleNum; i++) {
		labelArray[i] = i % C;
		class_i_Num[i % C]++;
	}
	*/
	
	//�����ʼ��������
	memset(class_i_m, 0, sizeof(double)*featureNum*C);//����
	for (i = 0; i<sampleNum; i++) {
		for (k = 0; k<C; k++) {
			if (*(labelArray + i) == k) {//��ǰ�����ǵ�k����
				for(j=0;j<featureNum;j++)
					class_i_m[k*featureNum + j] += *(featureArray + i*featureNum + j);
			}
		}
	}

	for (k = 0; k < C; k++) {//ÿ��������������
		for (j = 0; j<featureNum; j++)
			class_i_m[k*featureNum + j] /= class_i_Num[k];
	}
	//��������
	Kmeans_clustering(featureArray, labelArray, sampleNum, featureNum,  class_i_m, class_i_Num, C);
	
	delete []class_i_m;
	delete []class_i_Num;
}

