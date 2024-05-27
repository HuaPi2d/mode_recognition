#include "c_SVM.h"
#define feature 16 //特征数，共生矩阵是16个
//SVM训练,svm为需要优化的SVM，traindata为训练数据，label为标签，minC为C的最小范围, maxC为C的最大范围, stepC为步长
Ptr<SVM> getBestC_svm(Ptr<SVM> svm, double traindata[], int label[], int classNum, int trainNum, int featureNum, float minC, float maxC, float stepC)
{
	//构建训练集和验证集，输入的训练数据每个类别有16个样本，其中取12个构建训练集、4个构建验证集
	float traindata_2D[20 * 12][feature];//训练集12张图
	float validatedata_2D[20 * 4][feature];//验证集4张图
	int trainLabel[20 * 12];//训练集标号
	int validateLabel[20 * 4];//验证集标号
	for (int c = 0; c < classNum; c++) {
		for (int i = 0; i < trainNum; i++) {
			if (i <12) {
				for (int j = 0; j < featureNum; j++) 
					traindata_2D[c*(trainNum - 4) + i][j] = traindata[(c*trainNum + i)*featureNum + j];
				trainLabel[c*(trainNum - 4) + i] = label[(c*trainNum + i)];
			}
			else {
				for (int j = 0; j < featureNum; j++) 
					validatedata_2D[c*(trainNum - 12) + i -12][j] = traindata[(c*trainNum + i)*featureNum + j];
				validateLabel[c*(trainNum - 12) + i - 12] = label[(c*trainNum + i)];
			}
		}
	}
	//将训练集数据和标签转为Mat类型
	Mat trainDataMat(classNum*(trainNum-4), featureNum, CV_32FC1, traindata_2D);
	Mat trainLabelMat(classNum*(trainNum-4), 1, CV_32SC1, trainLabel);

	//遍历参数C,根据验证集判断最优参数C
	cout << "开始优化..." << endl;
	Ptr<SVM> bestSVM;//存放最优SVM
	int correctCount = 0, tmpCorrectCount;//存放统计的验证正确的个数
	for (double C = minC; C <= maxC; C = C + stepC) {
		Ptr<SVM> tmpSVM;//存放临时SVM
		tmpSVM = SVM::create();
		//根据输入的SVM，设置临时SVM的参数
		tmpSVM->setType(svm->getType());
		tmpSVM->setKernel(svm->getKernelType());
		tmpSVM->setGamma(svm->getGamma());
		//按此时的参数C设置
		tmpSVM->setC(C);
		int times = 1000;//设置迭代次数
		double error = 0.0001;//设置精度
		tmpSVM->setTermCriteria(TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, times, error));//设置训练终止条件，满足迭代次数和精度是终止
		tmpSVM->train(trainDataMat, ROW_SAMPLE, trainLabelMat);//训练SVM
		tmpCorrectCount = 0;
		for (int i = 0; i < classNum*(trainNum - 12); i++) {
			Mat validateDataMat(1, featureNum, CV_32FC1, validatedata_2D[i]);
			int result = tmpSVM->predict(validateDataMat);
			if (result == validateLabel[i])
				tmpCorrectCount++;
		}
		//输出本轮次SVM验证集准确率
		printf("C=%.5lf时验证集正确预测的个数： %d\n", C, tmpCorrectCount);
		if (tmpCorrectCount > correctCount) {
			correctCount = tmpCorrectCount;
			bestSVM = tmpSVM;//此时将临时tmpSVM保存到bestSVM
		}
	}
	cout << "优化结束\n最优参数C为：" << bestSVM->getC() << endl;
	return bestSVM;
}

//预测，svm为训练好的SVM，input是测试数据，output为每一类的预测结果
void SVM_predict(Ptr<SVM> svm, float input[], int classNum, int featureNum, int output[])
{
	Mat testMat(1, featureNum, CV_32FC1, input);//将input转换为Mat类型
	int result = svm->predict(testMat);//预测input，结果在result，得到类别号
	for (int i = 0; i < classNum; i++) {
		if (i == result - 1)//因为数组是从0开始
			output[i] = 1;
		else
			output[i] = 0;
	}	
}

//保存SVM模型
void save_svm(Ptr<SVM> svm)//保存支持向量
{
	svm->save("svm.xml");
}

//载入SVM模型
Ptr<SVM> load_svm()
{
	Ptr<SVM> svm;
	svm = StatModel::load<SVM>("svm.xml");//载入已经训练好的SVM分类器
	return svm;
}

void SVM_Classify(double traindata[], int trainLabel[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double P[][20])
{
	//SVM初始化
	Ptr<SVM> svm;
	svm = SVM::create();
	svm->setType(SVM::C_SVC);//设置类型为C类支持向量分类机，允许用异常值惩罚因子C进行不完全分类
	svm->setKernel(SVM::RBF);//设置核类型为径向基核函数RBF，线性核 SVM::LINEAR，还可以有其他的核
	svm->setGamma(3.5);//
	//超参数优化，针对参数C进行优化
	float minC = 1.0, maxC = 4.0, stepC = 0.1;//设置C参数得范围与步长
	svm = getBestC_svm(svm, traindata, trainLabel, classNum, trainNum, featureNum, minC, maxC, stepC);//寻找最优参数C对应的SVM模型
	save_svm(svm); //模型存储
	svm = load_svm();//模型加载

	float input[feature];//网络输入
	int output[20];//网络输出  
	for (int i = 0; i < classNum*testNum; i++) {
		for (int j = 0; j < featureNum; j++)
			input[j] = testdata[i*featureNum + j];
		SVM_predict(svm, input, classNum, featureNum, output);
		for (int c = 0; c < classNum; c++)
			P[i][c] = output[c];
	}
}