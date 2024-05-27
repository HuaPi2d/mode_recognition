#pragma warning(disable:4996)
#include "BP.h"

#define  innodeNum 16  //������������ǰ��������Ҫ��Ӧ��
#define  hidenodeNum 100//���������  
#define  outnodeNum 20 //��������  

/*ѵ��ǰ��ʼ����input_XΪ����ѵ�����������ݣ�output_YΪ����ѵ���ı�ǩ��
w_in2hide��w_hide2out��b_hide��b_outΪ�����Ȩ�غ�ƫ�ò�����
rate_w_in2hide��rate_w_hide2out��rate_b_hide��rate_b_outΪѵ��ѧϰ��*/
void BP_Init(double traindata[], int trainLabel[], int classNum, int trainNum, int featureNum, 
	double input_X[][innodeNum], double output_Y[][outnodeNum], 
	double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], 
	double &rate_w_in2hide, double &rate_w_hide2out, double &rate_b_hide, double &rate_b_out)
{
	//����ѵ���������ݺͱ�ǩ��ʼ��
	for (int i = 0; i < classNum*trainNum; i++) {
		for (int j = 0; j < featureNum; j++)
			input_X[i][j] = traindata[i*featureNum + j];
	}
	for (int i = 0; i < classNum*trainNum; i++) {
		for (int j = 0; j < classNum; j++) {
			if (j == (trainLabel[i] - 1))
				output_Y[i][j] = 1;
			else
				output_Y[i][j] = 0;
		}
	}

	//Ȩ�غ�ƫ�ó�ʼ���������ʼ��
	srand((int)time(0));
	for (int i = 0; i < innodeNum; i++) {
		for (int j = 0; j < hidenodeNum; j++)
			w_in2hide[i][j] = (2.0*(double)rand() / RAND_MAX) - 1;
	}
	srand((int)time(0));
	for (int i = 0; i < hidenodeNum; i++) {
		for (int j = 0; j < outnodeNum; j++)
			w_hide2out[i][j] = (2.0*(double)rand() / RAND_MAX) - 1;
	}
	srand((int)time(0));
	for (int i = 0; i < hidenodeNum; i++)
		b_hide[i] = (2.0*(double)rand() / RAND_MAX) - 1;
	srand((int)time(0));
	for (int i = 0; i < outnodeNum; i++)
		b_out[i] = (2.0*(double)rand() / RAND_MAX) - 1;

	//�������ѧϰ�ʳ�ʼ��
	rate_w_in2hide = 0.1;  //Ȩֵѧϰ�ʣ������--������)  
	rate_w_hide2out = 0.1; //Ȩֵѧϰ�� (������--�����)  
	rate_b_hide = 0.1; //�����㷧ֵѧϰ��  
	rate_b_out = 0.1; //����㷧ֵѧϰ�� 
}


//����ѵ����input_XΪѵ�����ݣ�output_YΪѵ����ǩ��w_in2hide��w_hide2out��b_hide��b_outΪ�����Ȩ�غ�ƫ�ò�����rate_w_in2hide��rate_w_hide2out��rate_b_hide��rate_b_outΪѵ��ѧϰ��
double train(double input_X[][innodeNum], double output_Y[][outnodeNum], int classNum, int trainNum, 
	double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], 
	double rate_w_in2hide, double rate_w_hide2out, double rate_b_hide, double rate_b_out)
{
	double e_hide[hidenodeNum];//��������У�����  
	double dev_out[outnodeNum];//ϣ�����ֵ��ʵ�����ֵ��ƫ��  
	double true_out[outnodeNum];//ϣ�����ֵ  
	double x[innodeNum]; //��������  
	double x_hide[hidenodeNum];//�������״ֵ̬  
	double x_out[outnodeNum];//������״ֵ̬  
	double actvalue_hide[hidenodeNum];//�����㼤��ֵ  
	double actvalue_out[hidenodeNum];//����㼤��ֵ  
	double error = 0.0;//�������

	for (int isamp = 0; isamp<classNum*trainNum; isamp++)//ѭ��ѵ��һ����Ʒ  
	{
		for (int i = 0; i < innodeNum; i++) {
			x[i] = input_X[isamp][i]; //���������
		}  
		for (int i = 0; i < outnodeNum; i++) {
			true_out[i] = output_Y[isamp][i]; //�������������  
		}
		//����ÿ����Ʒ������������׼  
		for (int j = 0; j<hidenodeNum; j++)
		{
			actvalue_hide[j] = 0.0;
			for (int i = 0; i < innodeNum; i++) {
				actvalue_hide[j] = actvalue_hide[j] + w_in2hide[i][j] * x[i];//���������Ԫ���뼤��ֵ  
				x_hide[j] = 1.0 / (1 + exp(-actvalue_hide[j] - b_hide[j]));//���������Ԫ�����  
			}  
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			actvalue_out[k] = 0.0;
			for (int j = 0; j < hidenodeNum; j++) {
				actvalue_out[k] = actvalue_out[k] + w_hide2out[j][k] * x_hide[j]; //��������Ԫ���뼤��ֵ  
				x_out[k] = 1.0 / (1.0 + exp(-actvalue_out[k] - b_out[k])); //��������Ԫ���  
			}
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			dev_out[k] = (true_out[k] - x_out[k])*x_out[k] * (1 - x_out[k]); //ϣ�������ʵ�������ƫ��  
			for (int j = 0; j < hidenodeNum; j++) {
				w_hide2out[j][k] += rate_w_hide2out*dev_out[k] * x_hide[j];  //��һ�ε�������������֮���������Ȩ
			}  
		}
		for (int j = 0; j<hidenodeNum; j++)
		{
			e_hide[j] = 0.0;
			for (int k = 0; k < outnodeNum; k++) {
				e_hide[j] = e_hide[j] + dev_out[k] * w_hide2out[j][k];  
			}
			e_hide[j] = e_hide[j] * x_hide[j] * (1 - x_hide[j]); //�������У�����
			for (int i = 0; i < innodeNum; i++) {
				w_in2hide[i][j] += rate_w_in2hide*e_hide[j] * x[i]; //��һ�ε�������������֮���������Ȩ
			}  
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			error += fabs(true_out[k] - x_out[k])*fabs(true_out[k] - x_out[k]); //���������  
		}
		for (int k = 0; k < outnodeNum; k++) {
			b_out[k] = b_out[k] + rate_b_out*dev_out[k]; //��һ�ε�������������֮�������ֵ 
		} 
		for (int j = 0; j < hidenodeNum; j++) {
			b_hide[j] = b_hide[j] + rate_b_hide*e_hide[j]; //��һ�ε�������������֮�������ֵ
		}  
	}
	error /= classNum*trainNum*outnodeNum;
	return error;
}

//Ԥ�⣬inputΪԤ�����ݣ�w_in2hide��w_hide2out��b_hide��b_outΪ�����Ȩ�غ�ƫ�ò�����outputΪԤ����������ÿһ��ĸ���
void BP_predict(double input[innodeNum], double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], double output[outnodeNum])
{ 
	double x_hide[hidenodeNum]; //�������״ֵ̬    
	double actvalue_hide[hidenodeNum]; //�����㼤��ֵ  
	double actvalue_out[hidenodeNum]; //����㼤��ֵ  
	for (int j = 0; j<hidenodeNum; j++)
	{
		actvalue_hide[j] = 0.0;
		for (int i = 0; i < innodeNum; i++) {
			actvalue_hide[j] = actvalue_hide[j] + w_in2hide[i][j] * input[i]; //���������Ԫ����ֵ  
			x_hide[j] = 1.0 / (1.0 + exp(-actvalue_hide[j] - b_hide[j])); //���������Ԫ���   
		}
	}
	for (int k = 0; k<outnodeNum; k++)
	{
		actvalue_out[k] = 0.0;
		for (int j = 0; j < hidenodeNum; j++) {
			actvalue_out[k] = actvalue_out[k] + w_hide2out[j][k] * x_hide[j];//��������Ԫ����ֵ  
			output[k] = 1.0 / (1.0 + exp(-actvalue_out[k] - b_out[k]));//��������Ԫ���  
		}  
	}
}

//��������ģ�Ͳ���
void save_Model(double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum])
{
	FILE *stream0;
	FILE *stream1;
	FILE *stream2;
	FILE *stream3;
	int i, j;
	//�������Ȩֵд��  
	if ((stream0 = fopen("w_in2hide.txt", "w+")) == NULL)
	{
		cout << "�����ļ�ʧ��!";
		exit(1);
	}
	for (i = 0; i<innodeNum; i++)
	{
		for (j = 0; j<hidenodeNum; j++)
		{
			fprintf(stream0, "%f\n", w_in2hide[i][j]);
		}
	}
	fclose(stream0);
	//������Ȩֵд��  
	if ((stream1 = fopen("w_hide2out.txt", "w+")) == NULL)
	{
		cout << "�����ļ�ʧ��!";
		exit(1);
	}
	for (i = 0; i<hidenodeNum; i++)
	{
		for (j = 0; j<outnodeNum; j++)
		{
			fprintf(stream1, "%f\n", w_hide2out[i][j]);
		}
	}
	fclose(stream1);
	//������㷧ֵд��  
	if ((stream2 = fopen("b_hide.txt", "w+")) == NULL)
	{
		cout << "�����ļ�ʧ��!";
		exit(1);
	}
	for (i = 0; i < hidenodeNum; i++) {
		fprintf(stream2, "%f\n", b_hide[i]);
	}
	fclose(stream2);
	//�����㷧ֵд��  
	if ((stream3 = fopen("b_out.txt", "w+")) == NULL)
	{
		cout << "�����ļ�ʧ��!";
		exit(1);
	}
	for (i = 0; i < outnodeNum; i++) {
		fprintf(stream3, "%f\n", b_out[i]);
	}
	fclose(stream3);
}

//��������ģ�Ͳ���
void load_Model(double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum])
{
	FILE *stream0;
	FILE *stream1;
	FILE *stream2;
	FILE *stream3;
	int i, j;
	//�������Ȩֵ����  
	if ((stream0 = fopen("w_in2hide.txt", "r")) == NULL)
	{
		cout << "���ļ�ʧ��!";
		exit(1);
	}
	float  wx_in2hide[innodeNum][hidenodeNum];
	for (i = 0; i<innodeNum; i++)
	{
		for (j = 0; j<hidenodeNum; j++)
		{
			fscanf(stream0, "%f", &wx_in2hide[i][j]);
			w_in2hide[i][j] = wx_in2hide[i][j];
		}
	}
	fclose(stream0);
	//������Ȩֵ����  
	if ((stream1 = fopen("w_hide2out.txt", "r")) == NULL)
	{
		cout << "���ļ�ʧ��!";
		exit(1);
	}
	float  wx_hide2out[hidenodeNum][outnodeNum];
	for (i = 0; i<hidenodeNum; i++)
	{
		for (j = 0; j<outnodeNum; j++)
		{
			fscanf(stream1, "%f", &wx_hide2out[i][j]);
			w_hide2out[i][j] = wx_hide2out[i][j];
		}
	}
	fclose(stream1);
	//������㷧ֵ����  
	if ((stream2 = fopen("b_hide.txt", "r")) == NULL)
	{
		cout << "���ļ�ʧ��!";
		exit(1);
	}
	float bx_hide[hidenodeNum];
	for (i = 0; i<hidenodeNum; i++)
	{
		fscanf(stream2, "%f", &bx_hide[i]);
		b_hide[i] = bx_hide[i];
	}
	fclose(stream2);
	//�����㷧ֵ����  
	if ((stream3 = fopen("b_out.txt", "r")) == NULL)
	{
		cout << "���ļ�ʧ��!";
		exit(1);
	}
	float bx_out[outnodeNum];
	for (i = 0; i<outnodeNum; i++)
	{
		fscanf(stream3, "%f", &bx_out[i]);
		b_out[i] = bx_out[i];
	}
	fclose(stream3);
}

void BP_Classify(double traindata[], int trainLabel[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double* test_P, double* train_P)
{
	double input_X[20 * 16][innodeNum];     //����ѵ������������
	double output_Y[20 * 16][outnodeNum];    //����ѵ���ı�ǩ

	double w_in2hide[innodeNum][hidenodeNum];//�������Ȩֵ  
	double w_hide2out[hidenodeNum][outnodeNum];//������Ȩֵ  
	double b_hide[hidenodeNum];//������㷧ֵ  
	double b_out[outnodeNum];//�����㷧ֵ  

	double rate_w_in2hide;  //Ȩֵѧϰ�ʣ������--������)  
	double rate_w_hide2out; //Ȩֵѧϰ�� (������--�����)  
	double rate_b_hide; //�����㷧ֵѧϰ��  
	double rate_b_out; //����㷧ֵѧϰ�� 

	//ѵ��ǰ��ʼ��
	BP_Init(traindata, trainLabel, classNum, trainNum, featureNum,
		input_X, output_Y,
		w_in2hide, w_hide2out, b_hide, b_out,
		rate_w_in2hide, rate_w_hide2out, rate_b_hide, rate_b_out);

	//ѵ������ģ��
	printf("��ʼѵ��...");
	int round = 0;//ͳ��ѵ������
	double error;//ѵ�����
	while (round < 1000) {//��ѵ��1000��
		error = train(input_X, output_Y, classNum, trainNum, w_in2hide, w_hide2out, b_hide, b_out, rate_w_in2hide, rate_w_hide2out, rate_b_hide, rate_b_out);
		round++;
		printf("��%d��ѵ����%10f\n", round, error);
	}
	printf("ѵ������");

	save_Model(w_in2hide, w_hide2out, b_hide, b_out);//�����������,����ѵ��������׼ȷ�ʵ�ʱ��Ҫ���ⲿ������

	load_Model(w_in2hide, w_hide2out, b_hide, b_out);//�����������

	//�����������ܣ����Խ��������P[][20]��
	double input[innodeNum];//��������
	double output[outnodeNum];//�������  
	for (int i = 0; i < classNum * testNum; i++) {
		for (int j = 0; j < featureNum; j++)
			input[j] = testdata[i * featureNum + j];
		BP_predict(input, w_in2hide, w_hide2out, b_hide, b_out, output);
		for (int c = 0; c < classNum; c++)
			test_P[i * classNum + c] = output[c];
	}

	for (int i = 0; i < classNum * trainNum; i++)
	{
		for (int j = 0; j < featureNum; j++) {
			input[j] = traindata[i * featureNum + j];
		}
		BP_predict(input, w_in2hide, w_hide2out, b_hide, b_out, output);
		for (int c = 0; c < classNum; c++) {
			train_P[i * classNum + c] = output[c];
		}
	}
}