#include "c_SVM.h"
#define feature 16 //������������������16��
//SVMѵ��,svmΪ��Ҫ�Ż���SVM��traindataΪѵ�����ݣ�labelΪ��ǩ��minCΪC����С��Χ, maxCΪC�����Χ, stepCΪ����
Ptr<SVM> getBestC_svm(Ptr<SVM> svm, double traindata[], int label[], int classNum, int trainNum, int featureNum, float minC, float maxC, float stepC)
{
	//����ѵ��������֤���������ѵ������ÿ�������16������������ȡ12������ѵ������4��������֤��
	float traindata_2D[20 * 12][feature];//ѵ����12��ͼ
	float validatedata_2D[20 * 4][feature];//��֤��4��ͼ
	int trainLabel[20 * 12];//ѵ�������
	int validateLabel[20 * 4];//��֤�����
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
	//��ѵ�������ݺͱ�ǩתΪMat����
	Mat trainDataMat(classNum*(trainNum-4), featureNum, CV_32FC1, traindata_2D);
	Mat trainLabelMat(classNum*(trainNum-4), 1, CV_32SC1, trainLabel);

	//��������C,������֤���ж����Ų���C
	cout << "��ʼ�Ż�..." << endl;
	Ptr<SVM> bestSVM;//�������SVM
	int correctCount = 0, tmpCorrectCount;//���ͳ�Ƶ���֤��ȷ�ĸ���
	for (double C = minC; C <= maxC; C = C + stepC) {
		Ptr<SVM> tmpSVM;//�����ʱSVM
		tmpSVM = SVM::create();
		//���������SVM��������ʱSVM�Ĳ���
		tmpSVM->setType(svm->getType());
		tmpSVM->setKernel(svm->getKernelType());
		tmpSVM->setGamma(svm->getGamma());
		//����ʱ�Ĳ���C����
		tmpSVM->setC(C);
		int times = 1000;//���õ�������
		double error = 0.0001;//���þ���
		tmpSVM->setTermCriteria(TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, times, error));//����ѵ����ֹ������������������;�������ֹ
		tmpSVM->train(trainDataMat, ROW_SAMPLE, trainLabelMat);//ѵ��SVM
		tmpCorrectCount = 0;
		for (int i = 0; i < classNum*(trainNum - 12); i++) {
			Mat validateDataMat(1, featureNum, CV_32FC1, validatedata_2D[i]);
			int result = tmpSVM->predict(validateDataMat);
			if (result == validateLabel[i])
				tmpCorrectCount++;
		}
		//������ִ�SVM��֤��׼ȷ��
		printf("C=%.5lfʱ��֤����ȷԤ��ĸ����� %d\n", C, tmpCorrectCount);
		if (tmpCorrectCount > correctCount) {
			correctCount = tmpCorrectCount;
			bestSVM = tmpSVM;//��ʱ����ʱtmpSVM���浽bestSVM
		}
	}
	cout << "�Ż�����\n���Ų���CΪ��" << bestSVM->getC() << endl;
	return bestSVM;
}

//Ԥ�⣬svmΪѵ���õ�SVM��input�ǲ������ݣ�outputΪÿһ���Ԥ����
void SVM_predict(Ptr<SVM> svm, float input[], int classNum, int featureNum, int output[])
{
	Mat testMat(1, featureNum, CV_32FC1, input);//��inputת��ΪMat����
	int result = svm->predict(testMat);//Ԥ��input�������result���õ�����
	for (int i = 0; i < classNum; i++) {
		if (i == result - 1)//��Ϊ�����Ǵ�0��ʼ
			output[i] = 1;
		else
			output[i] = 0;
	}	
}

//����SVMģ��
void save_svm(Ptr<SVM> svm)//����֧������
{
	svm->save("svm.xml");
}

//����SVMģ��
Ptr<SVM> load_svm()
{
	Ptr<SVM> svm;
	svm = StatModel::load<SVM>("svm.xml");//�����Ѿ�ѵ���õ�SVM������
	return svm;
}

void SVM_Classify(double traindata[], int trainLabel[], double testdata[], int classNum, int trainNum, int testNum, int featureNum, double P[][20])
{
	//SVM��ʼ��
	Ptr<SVM> svm;
	svm = SVM::create();
	svm->setType(SVM::C_SVC);//��������ΪC��֧��������������������쳣ֵ�ͷ�����C���в���ȫ����
	svm->setKernel(SVM::RBF);//���ú�����Ϊ������˺���RBF�����Ժ� SVM::LINEAR���������������ĺ�
	svm->setGamma(3.5);//
	//�������Ż�����Բ���C�����Ż�
	float minC = 1.0, maxC = 4.0, stepC = 0.1;//����C�����÷�Χ�벽��
	svm = getBestC_svm(svm, traindata, trainLabel, classNum, trainNum, featureNum, minC, maxC, stepC);//Ѱ�����Ų���C��Ӧ��SVMģ��
	save_svm(svm); //ģ�ʹ洢
	svm = load_svm();//ģ�ͼ���

	float input[feature];//��������
	int output[20];//�������  
	for (int i = 0; i < classNum*testNum; i++) {
		for (int j = 0; j < featureNum; j++)
			input[j] = testdata[i*featureNum + j];
		SVM_predict(svm, input, classNum, featureNum, output);
		for (int c = 0; c < classNum; c++)
			P[i][c] = output[c];
	}
}