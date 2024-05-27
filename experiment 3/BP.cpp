#pragma warning(disable:4996)
#include "BP.h"

#define  innodeNum 16  //输入结点数，跟前边特征数要对应上
#define  hidenodeNum 100//隐含结点数  
#define  outnodeNum 20 //输出结点数  

/*训练前初始化，input_X为网络训练的输入数据，output_Y为网络训练的标签，
w_in2hide、w_hide2out、b_hide、b_out为网络的权重和偏置参数，
rate_w_in2hide、rate_w_hide2out、rate_b_hide、rate_b_out为训练学习率*/
void BP_Init(double traindata[], int trainLabel[], int classNum, int trainNum, int featureNum, 
	double input_X[][innodeNum], double output_Y[][outnodeNum], 
	double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], 
	double &rate_w_in2hide, double &rate_w_hide2out, double &rate_b_hide, double &rate_b_out)
{
	//网络训练输入数据和标签初始化
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

	//权重和偏置初始化，随机初始化
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

	//网络参数学习率初始化
	rate_w_in2hide = 0.1;  //权值学习率（输入层--隐含层)  
	rate_w_hide2out = 0.1; //权值学习率 (隐含层--输出层)  
	rate_b_hide = 0.1; //隐含层阀值学习率  
	rate_b_out = 0.1; //输出层阀值学习率 
}


//网络训练，input_X为训练数据，output_Y为训练标签，w_in2hide、w_hide2out、b_hide、b_out为网络的权重和偏置参数，rate_w_in2hide、rate_w_hide2out、rate_b_hide、rate_b_out为训练学习率
double train(double input_X[][innodeNum], double output_Y[][outnodeNum], int classNum, int trainNum, 
	double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], 
	double rate_w_in2hide, double rate_w_hide2out, double rate_b_hide, double rate_b_out)
{
	double e_hide[hidenodeNum];//隐含结点的校正误差  
	double dev_out[outnodeNum];//希望输出值与实际输出值的偏差  
	double true_out[outnodeNum];//希望输出值  
	double x[innodeNum]; //输入向量  
	double x_hide[hidenodeNum];//隐含结点状态值  
	double x_out[outnodeNum];//输出结点状态值  
	double actvalue_hide[hidenodeNum];//隐含层激活值  
	double actvalue_out[hidenodeNum];//输出层激活值  
	double error = 0.0;//均方误差

	for (int isamp = 0; isamp<classNum*trainNum; isamp++)//循环训练一次样品  
	{
		for (int i = 0; i < innodeNum; i++) {
			x[i] = input_X[isamp][i]; //输入的样本
		}  
		for (int i = 0; i < outnodeNum; i++) {
			true_out[i] = output_Y[isamp][i]; //期望输出的样本  
		}
		//构造每个样品的输入和输出标准  
		for (int j = 0; j<hidenodeNum; j++)
		{
			actvalue_hide[j] = 0.0;
			for (int i = 0; i < innodeNum; i++) {
				actvalue_hide[j] = actvalue_hide[j] + w_in2hide[i][j] * x[i];//隐含层各单元输入激活值  
				x_hide[j] = 1.0 / (1 + exp(-actvalue_hide[j] - b_hide[j]));//隐含层各单元的输出  
			}  
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			actvalue_out[k] = 0.0;
			for (int j = 0; j < hidenodeNum; j++) {
				actvalue_out[k] = actvalue_out[k] + w_hide2out[j][k] * x_hide[j]; //输出层各单元输入激活值  
				x_out[k] = 1.0 / (1.0 + exp(-actvalue_out[k] - b_out[k])); //输出层各单元输出  
			}
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			dev_out[k] = (true_out[k] - x_out[k])*x_out[k] * (1 - x_out[k]); //希望输出与实际输出的偏差  
			for (int j = 0; j < hidenodeNum; j++) {
				w_hide2out[j][k] += rate_w_hide2out*dev_out[k] * x_hide[j];  //下一次的隐含层和输出层之间的新连接权
			}  
		}
		for (int j = 0; j<hidenodeNum; j++)
		{
			e_hide[j] = 0.0;
			for (int k = 0; k < outnodeNum; k++) {
				e_hide[j] = e_hide[j] + dev_out[k] * w_hide2out[j][k];  
			}
			e_hide[j] = e_hide[j] * x_hide[j] * (1 - x_hide[j]); //隐含层的校正误差
			for (int i = 0; i < innodeNum; i++) {
				w_in2hide[i][j] += rate_w_in2hide*e_hide[j] * x[i]; //下一次的输入层和隐含层之间的新连接权
			}  
		}
		for (int k = 0; k<outnodeNum; k++)
		{
			error += fabs(true_out[k] - x_out[k])*fabs(true_out[k] - x_out[k]); //计算均方差  
		}
		for (int k = 0; k < outnodeNum; k++) {
			b_out[k] = b_out[k] + rate_b_out*dev_out[k]; //下一次的隐含层和输出层之间的新阈值 
		} 
		for (int j = 0; j < hidenodeNum; j++) {
			b_hide[j] = b_hide[j] + rate_b_hide*e_hide[j]; //下一次的输入层和隐含层之间的新阈值
		}  
	}
	error /= classNum*trainNum*outnodeNum;
	return error;
}

//预测，input为预测数据，w_in2hide、w_hide2out、b_hide、b_out为网络的权重和偏置参数，output为预测结果，属于每一类的概率
void BP_predict(double input[innodeNum], double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum], double output[outnodeNum])
{ 
	double x_hide[hidenodeNum]; //隐含结点状态值    
	double actvalue_hide[hidenodeNum]; //隐含层激活值  
	double actvalue_out[hidenodeNum]; //输出层激活值  
	for (int j = 0; j<hidenodeNum; j++)
	{
		actvalue_hide[j] = 0.0;
		for (int i = 0; i < innodeNum; i++) {
			actvalue_hide[j] = actvalue_hide[j] + w_in2hide[i][j] * input[i]; //隐含层各单元激活值  
			x_hide[j] = 1.0 / (1.0 + exp(-actvalue_hide[j] - b_hide[j])); //隐含层各单元输出   
		}
	}
	for (int k = 0; k<outnodeNum; k++)
	{
		actvalue_out[k] = 0.0;
		for (int j = 0; j < hidenodeNum; j++) {
			actvalue_out[k] = actvalue_out[k] + w_hide2out[j][k] * x_hide[j];//输出层各单元激活值  
			output[k] = 1.0 / (1.0 + exp(-actvalue_out[k] - b_out[k]));//输出层各单元输出  
		}  
	}
}

//保存网络模型参数
void save_Model(double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum])
{
	FILE *stream0;
	FILE *stream1;
	FILE *stream2;
	FILE *stream3;
	int i, j;
	//隐含结点权值写入  
	if ((stream0 = fopen("w_in2hide.txt", "w+")) == NULL)
	{
		cout << "创建文件失败!";
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
	//输出结点权值写入  
	if ((stream1 = fopen("w_hide2out.txt", "w+")) == NULL)
	{
		cout << "创建文件失败!";
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
	//隐含结点阀值写入  
	if ((stream2 = fopen("b_hide.txt", "w+")) == NULL)
	{
		cout << "创建文件失败!";
		exit(1);
	}
	for (i = 0; i < hidenodeNum; i++) {
		fprintf(stream2, "%f\n", b_hide[i]);
	}
	fclose(stream2);
	//输出结点阀值写入  
	if ((stream3 = fopen("b_out.txt", "w+")) == NULL)
	{
		cout << "创建文件失败!";
		exit(1);
	}
	for (i = 0; i < outnodeNum; i++) {
		fprintf(stream3, "%f\n", b_out[i]);
	}
	fclose(stream3);
}

//载入网络模型参数
void load_Model(double w_in2hide[innodeNum][hidenodeNum], double w_hide2out[hidenodeNum][outnodeNum], double b_hide[hidenodeNum], double b_out[outnodeNum])
{
	FILE *stream0;
	FILE *stream1;
	FILE *stream2;
	FILE *stream3;
	int i, j;
	//隐含结点权值读出  
	if ((stream0 = fopen("w_in2hide.txt", "r")) == NULL)
	{
		cout << "打开文件失败!";
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
	//输出结点权值读出  
	if ((stream1 = fopen("w_hide2out.txt", "r")) == NULL)
	{
		cout << "打开文件失败!";
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
	//隐含结点阀值读出  
	if ((stream2 = fopen("b_hide.txt", "r")) == NULL)
	{
		cout << "打开文件失败!";
		exit(1);
	}
	float bx_hide[hidenodeNum];
	for (i = 0; i<hidenodeNum; i++)
	{
		fscanf(stream2, "%f", &bx_hide[i]);
		b_hide[i] = bx_hide[i];
	}
	fclose(stream2);
	//输出结点阀值读出  
	if ((stream3 = fopen("b_out.txt", "r")) == NULL)
	{
		cout << "打开文件失败!";
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
	double input_X[20 * 16][innodeNum];     //网络训练的输入数据
	double output_Y[20 * 16][outnodeNum];    //网络训练的标签

	double w_in2hide[innodeNum][hidenodeNum];//隐含结点权值  
	double w_hide2out[hidenodeNum][outnodeNum];//输出结点权值  
	double b_hide[hidenodeNum];//隐含结点阀值  
	double b_out[outnodeNum];//输出结点阀值  

	double rate_w_in2hide;  //权值学习率（输入层--隐含层)  
	double rate_w_hide2out; //权值学习率 (隐含层--输出层)  
	double rate_b_hide; //隐含层阀值学习率  
	double rate_b_out; //输出层阀值学习率 

	//训练前初始化
	BP_Init(traindata, trainLabel, classNum, trainNum, featureNum,
		input_X, output_Y,
		w_in2hide, w_hide2out, b_hide, b_out,
		rate_w_in2hide, rate_w_hide2out, rate_b_hide, rate_b_out);

	//训练网络模型
	printf("开始训练...");
	int round = 0;//统计训练轮数
	double error;//训练误差
	while (round < 1000) {//共训练1000轮
		error = train(input_X, output_Y, classNum, trainNum, w_in2hide, w_hide2out, b_hide, b_out, rate_w_in2hide, rate_w_hide2out, rate_b_hide, rate_b_out);
		round++;
		printf("第%d轮训练误差：%10f\n", round, error);
	}
	printf("训练结束");

	save_Model(w_in2hide, w_hide2out, b_hide, b_out);//保存网络参数,测试训练集分类准确率的时候要把这部分屏蔽

	load_Model(w_in2hide, w_hide2out, b_hide, b_out);//载入网络参数

	//测试网络性能，测试结果保存在P[][20]中
	double input[innodeNum];//网络输入
	double output[outnodeNum];//网络输出  
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