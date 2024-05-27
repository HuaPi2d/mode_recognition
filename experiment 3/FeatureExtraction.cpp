#pragma warning(disable:4996)

#include "FeatureExtraction.h"
#include "Gabor.h"

//��ȡ��������������Ҷȹ�������4������
#define GLCM_DIS 1 //�Ҷȹ��������ͳ�ƾ���
#define GLCM_CLASS 16 //�Ҷȹ�������ͼ��Ҷ�ֵ�ȼ���
#define GLCM_ANGLE_0 0//ˮƽ
#define GLCM_ANGLE_90 1//��ֱ
#define GLCM_ANGLE_45 45//45�ȶԽ�
#define GLCM_ANGLE_135 135//135�ȶԽ�

void get_GLCMMoment(Mat img, int angle, double GLCMMoment[4]) {
	int i, j;
	int height, width;
	height = img.rows;
	width = img.cols;
	double* glcm = new double[GLCM_CLASS * GLCM_CLASS];//�µĻҶȵȼ���������
	int* histimg = new int[width * height];//�µĻҶȵȼ�ͼ��

	//�Ҷȵȼ�������Ϊ16���ȼ�
	int relative_value = 0;
	int num;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			relative_value = img.at<Vec3b>(i, j)[0];
			num = (int)(relative_value * GLCM_CLASS / 256);
			histimg[i * width + j] = num;
			//printf("%d ",histImage[i * width + j]);
		}
	}

	//��ʼ���Ҷȹ�������
	for (i = 0; i < GLCM_CLASS; i++) {
		for (j = 0; j < GLCM_CLASS; j++) {
			glcm[i * GLCM_CLASS + j] = 0;
		}
	}

	//����Ҷȹ�������
	int w, k, l;
	if (angle == GLCM_ANGLE_0) {
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				l = histimg[i * width + j];
				if (j + GLCM_DIS >= 0 && j + GLCM_DIS < width)
				{
					k = histimg[i * width + j + GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
				if (j - GLCM_DIS >= 0 && j - GLCM_DIS < width)
				{
					k = histimg[i * width + j - GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
			}
		}
		//�ҶȾ����һ��
		double total = 0;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				total = total + glcm[i * GLCM_CLASS + j];
			}
		}
		total = total;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				glcm[i * GLCM_CLASS + j] = glcm[i * GLCM_CLASS + j] / total;
			}
		}
	}
	else if (angle == GLCM_ANGLE_90)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				l = histimg[i * width + j];
				if (i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
				{
					k = histimg[(i + GLCM_DIS) * width + j];
					glcm[l * GLCM_CLASS + k]++;
				}
				if (i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
				{
					k = histimg[(i - GLCM_DIS) * width + j];
					glcm[l * GLCM_CLASS + k]++;
				}
			}
		}
		//�ҶȾ����һ��
		double total = 0;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				total = total + glcm[i * GLCM_CLASS + j];
			}
		}
		total = total;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				glcm[i * GLCM_CLASS + j] = glcm[i * GLCM_CLASS + j] / total;
			}
		}
	}
	//135�ȶԽǷ���
	else if (angle == GLCM_ANGLE_135)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				l = histimg[i * width + j];

				if (j + GLCM_DIS >= 0 && j + GLCM_DIS < width && i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
				{
					k = histimg[(i + GLCM_DIS) * width + j + GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
				if (j - GLCM_DIS >= 0 && j - GLCM_DIS < width && i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
				{
					k = histimg[(i - GLCM_DIS) * width + j - GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
			}
		}
		//�ҶȾ����һ��
		double total = 0;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				total = total + glcm[i * GLCM_CLASS + j];
			}
		}
		total = total;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				glcm[i * GLCM_CLASS + j] = glcm[i * GLCM_CLASS + j] / total;
			}
		}
	}
	//45�ȶԽǷ���
	else if (angle == GLCM_ANGLE_45)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				l = histimg[i * width + j];

				if (j + GLCM_DIS >= 0 && j + GLCM_DIS < width && i - GLCM_DIS >= 0 && i - GLCM_DIS < height)
				{
					k = histimg[(i - GLCM_DIS) * width + j + GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
				if (j - GLCM_DIS >= 0 && j - GLCM_DIS < width && i + GLCM_DIS >= 0 && i + GLCM_DIS < height)
				{
					k = histimg[(i + GLCM_DIS) * width + j - GLCM_DIS];
					glcm[l * GLCM_CLASS + k]++;
				}
			}
		}
		//�ҶȾ����һ��
		double total = 0;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				total = total + glcm[i * GLCM_CLASS + j];
			}
		}
		total = total;
		for (i = 0; i < GLCM_CLASS; i++)
		{
			for (j = 0; j < GLCM_CLASS; j++)
			{
				glcm[i * GLCM_CLASS + j] = glcm[i * GLCM_CLASS + j] / total;
			}
		}
	}
	//��������ֵ
	double entropy = 0, energy = 0, contrast = 0, homogenity = 0;
	for (i = 0; i < GLCM_CLASS; i++)
	{
		for (j = 0; j < GLCM_CLASS; j++)
		{
			//��
			if (glcm[i * GLCM_CLASS + j] > 0)
				entropy -= glcm[i * GLCM_CLASS + j] * log10(glcm[i * GLCM_CLASS + j]);
			//����
			energy += glcm[i * GLCM_CLASS + j] * glcm[i * GLCM_CLASS + j];
			//�Աȶ�
			contrast += (i - j) * (i - j) * glcm[i * GLCM_CLASS + j];
			//һ����
			homogenity += 1.0 / (1 + (i - j) * (i - j)) * glcm[i * GLCM_CLASS + j];
		}
	}
	//�洢����ֵ
	GLCMMoment[0] = entropy;
	GLCMMoment[1] = energy;
	GLCMMoment[2] = contrast;
	GLCMMoment[3] = homogenity;
}

void FeatureExtraction::group_extractGLCMFeature() 
{
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double GLCMMoment0[4], GLCMMoment90[4], GLCMMoment45[4], GLCMMoment135[4];
	FILE* fp = fopen(featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j��ͼ��
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//��i��������Ŀ¼
			if (i >= 10) {//��������99��������������
				s[0] = i / 10 + 48;
				s[1] = i % 10 + 48;
				s[2] = '\0';
			}
			else {
				s[0] = i + 48;
				s[1] = '\0';
			}
			strcat(filename, s);
			strcat(filename, "/");
			//��i�����ֵĵ�j��ͼƬ
			if (j >= 10 && j < 100) {//��������999��������������
				s[0] = j / 10 + 48;
				s[1] = j % 10 + 48;
				s[2] = '\0';
			}
			else if (j < 10) {
				s[0] = j + 48;
				s[1] = '\0';
			}
			else {
				s[0] = j / 100 + 48;
				s[1] = (j % 100) / 10 + 48;
				s[2] = (j % 100) % 10 + 48;
				s[3] = '\0';
			}
			strcat(filename, s);
			strcat(filename, ".bmp");

			Img = imread(filename);
			get_GLCMMoment(Img, GLCM_ANGLE_0, GLCMMoment0);//��������������������ǩ
			get_GLCMMoment(Img, GLCM_ANGLE_45, GLCMMoment45);//��������������������ǩ
			get_GLCMMoment(Img, GLCM_ANGLE_90, GLCMMoment90);//��������������������ǩ
			get_GLCMMoment(Img, GLCM_ANGLE_135, GLCMMoment135);//��������������������ǩ


			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment0[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment45[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment90[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment135[d]);
			fprintf(fp, "%d\n", i);//��һ�����
		}
	}
	fclose(fp);
}

void get_GaborFeature(Mat img, double GaborFeature[80])//����ʽ����ͼ��
{
	int GaborNum = 40; //Gabor�˲�������
	int U = 8;   //8������
	int V = 5;   //5���߶�
	Mat  GaborReal[40];
	Mat  GaborImg[40];
	getAllGaborKernal(GaborNum, U, V, GaborReal, GaborImg);
	calculate_Feature(img, GaborReal, GaborImg, GaborNum, GaborFeature);
	for (int d = 0; d < 80; d++)
		GaborFeature[d] = GaborFeature[d];
}

void FeatureExtraction::group_extractGaborFeature() {
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double GaborFeature[80];
	FILE* fp = fopen(featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j����
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//��i������Ŀ¼
			if (i >= 10) {//����������99��������������
				s[0] = i / 10 + 48;
				s[1] = i % 10 + 48;
				s[2] = '\0';
			}
			else {
				s[0] = i + 48;
				s[1] = '\0';
			}
			strcat(filename, s);
			strcat(filename, "/");
			//��i���˵ĵ�j������
			if (j >= 10) {//������������99��������������
				s[0] = j / 10 + 48;
				s[1] = j % 10 + 48;
				s[2] = '\0';
			}
			else {
				s[0] = j + 48;
				s[1] = '\0';
			}
			strcat(filename, s);
			strcat(filename, ".bmp");

			Img = imread(filename);
			get_GaborFeature(Img, GaborFeature);//����Babor������������ǩ
			for (int d = 0; d < 80; d++)
				fprintf(fp, "%f ", GaborFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

void get_COLORFeature(Mat img, double COLORFeature[5])
{
	int width = img.cols;
	int height = img.rows;
	// ���Ҷ�ͼ��ת��Ϊ��һ���ĸ�����ͼ��  
	img.convertTo(img, CV_32F);
	normalize(img, img, 0, 1, NORM_MINMAX);
	// �����ֵ�ͱ�׼��  
	Scalar mean;
	Scalar stddev;
	meanStdDev(img, mean, stddev);
	// ��һ���������Ҷ�ͼ��ľ�ֵ  
	COLORFeature[0] = mean[0];
	// �ڶ����������Ҷ�ͼ��ı�׼��  
	COLORFeature[1] = stddev[0];
	// �������������Ҷ�ͼ���ƫ��  
	double skewness = 0.0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			skewness += pow((img.at<float>(i, j) - mean[0]), 3) / (height * width);
		}
	}
	skewness = (skewness < 0) ? -pow(-skewness, 1.0 / 3) : pow(skewness, 1.0 / 3);
	COLORFeature[2] = skewness;
	// ���ĸ��������Ҷ�ͼ��ķ��  
	double kurtosis = 0.0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			kurtosis += pow((img.at<float>(i, j) - mean[0]), 4) / (height * width);
		}
	}
	kurtosis = kurtosis / (stddev[0] * stddev[0]) - 3;
	COLORFeature[3] = kurtosis;
	// ������������Ҷ�ֱ��ͼ����  
	double entropy = 0.0;
	Mat hist;
	int histSize = 256; // ֱ��ͼ�� bin ����  
	float range[] = { 0, 256 }; // �Ҷ�ֵ��Χ  
	const float* histRange = { range };
	calcHist(&img, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);
	for (int i = 0; i < histSize; i++) {
		double p = hist.at<float>(i) / (height * width);
		if (p > 0)
			entropy -= p * log2(p);
	}
	COLORFeature[4] = entropy;
}

void FeatureExtraction::group_extractCOLORFeature()
{
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double COLORFeature[3];
	FILE* fp = fopen(featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j��ͼ��
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//��i��������Ŀ¼
			if (i >= 10) {//��������99��������������
				s[0] = i / 10 + 48;
				s[1] = i % 10 + 48;
				s[2] = '\0';
			}
			else {
				s[0] = i + 48;
				s[1] = '\0';
			}
			strcat(filename, s);
			strcat(filename, "/");
			//��i�����ֵĵ�j��ͼƬ
			if (j >= 10 && j < 100) {//��������999��������������
				s[0] = j / 10 + 48;
				s[1] = j % 10 + 48;
				s[2] = '\0';
			}
			else if (j < 10) {
				s[0] = j + 48;
				s[1] = '\0';
			}
			else {
				s[0] = j / 100 + 48;
				s[1] = (j % 100) / 10 + 48;
				s[2] = (j % 100) % 10 + 48;
				s[3] = '\0';
			}
			strcat(filename, s);
			strcat(filename, ".bmp");

			Img = imread(filename);
			get_COLORFeature(Img, COLORFeature);
			for (int d = 0; d < 3; d++)
				fprintf(fp, "%f ", COLORFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

void get_SHAPEFeature(Mat img, double SHAPEFeature[6])
{
	// ��ֵ��ͼ��
	Mat binaryImage;
	threshold(img, binaryImage, 128, 255, THRESH_BINARY);

	// ����ˮƽ�봹ֱ�ݶ�
	Mat grad_x, grad_y;
	img.convertTo(img, CV_32F);
	normalize(img, img, 0, 1, NORM_MINMAX);
	Sobel(img, grad_x, CV_32F, 1, 0, 1);
	Sobel(img, grad_y, CV_32F, 0, 1, 1);

	// ������ݶȷ�ֵ�ͷ���
	Mat gradientMagnitude, gradientDirection;
	magnitude(grad_x, grad_y, gradientMagnitude);
	phase(grad_x, grad_y, gradientDirection, true);
	normalize(grad_x, grad_x, 0, 1, NORM_MINMAX);
	normalize(grad_y, grad_y, 0, 1, NORM_MINMAX);
	normalize(gradientDirection, gradientDirection, 0, 1, NORM_MINMAX);

	// ��״��������
	Scalar mean;
	Scalar sd;

	// ����ˮƽ��Ե����
	meanStdDev(grad_x, mean, sd);
	SHAPEFeature[0] = mean[0];
	SHAPEFeature[1] = sd[0];

	// ���㴹ֱ��Ե����
	meanStdDev(grad_y, mean, sd);
	SHAPEFeature[2] = mean[0];
	SHAPEFeature[3] = sd[0];

	// ����Ƕ�����
	meanStdDev(gradientDirection, mean, sd);
	SHAPEFeature[4] = mean[0];
	SHAPEFeature[5] = sd[0];
}

void FeatureExtraction::group_extractSHAPEFeature()
{
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double SHAPEFeature[3];
	FILE* fp = fopen(featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j��ͼ��
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//��i��������Ŀ¼
			if (i >= 10) {//��������99��������������
				s[0] = i / 10 + 48;
				s[1] = i % 10 + 48;
				s[2] = '\0';
			}
			else {
				s[0] = i + 48;
				s[1] = '\0';
			}
			strcat(filename, s);
			strcat(filename, "/");
			//��i�����ֵĵ�j��ͼƬ
			if (j >= 10 && j < 100) {//��������999��������������
				s[0] = j / 10 + 48;
				s[1] = j % 10 + 48;
				s[2] = '\0';
			}
			else if (j < 10) {
				s[0] = j + 48;
				s[1] = '\0';
			}
			else {
				s[0] = j / 100 + 48;
				s[1] = (j % 100) / 10 + 48;
				s[2] = (j % 100) % 10 + 48;
				s[3] = '\0';
			}
			strcat(filename, s);
			strcat(filename, ".bmp");
			//cout << filename << endl; D:/VS code/projects/ģʽʶ��ʵ��/ʵ��һ/���ǿ�/s16/4.bmp

			Img = imread(filename);
			get_SHAPEFeature(Img, SHAPEFeature);
			for (int d = 0; d < 3; d++)
				fprintf(fp, "%f ", SHAPEFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

void FeatureExtraction::group_extractFeature(int index) 
{
	if (featureOutputFile == nullptr) {
		featureOutputFile = new char[strlen(featureOutputFileRoot) + strlen(featureOutputFileName)];
		featureOutputFile = featureOutputFileRoot;
		strcat_s(featureOutputFile, featureOutputFileName);
	}
}