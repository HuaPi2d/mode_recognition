#pragma warning(disable:4996)

#include "FeatureExtraction.h"
#include "Gabor.h"

//提取纹理特征，计算灰度共生矩阵，4个特征
#define GLCM_DIS 1 //灰度共生矩阵的统计距离
#define GLCM_CLASS 16 //灰度共生矩阵图像灰度值等级化
#define GLCM_ANGLE_0 0//水平
#define GLCM_ANGLE_90 1//垂直
#define GLCM_ANGLE_45 45//45度对角
#define GLCM_ANGLE_135 135//135度对角

void get_GLCMMoment(Mat img, int angle, double GLCMMoment[4]) {
	int i, j;
	int height, width;
	height = img.rows;
	width = img.cols;
	double* glcm = new double[GLCM_CLASS * GLCM_CLASS];//新的灰度等级共生矩阵
	int* histimg = new int[width * height];//新的灰度等级图像

	//灰度等级化，分为16个等级
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

	//初始化灰度共生矩阵
	for (i = 0; i < GLCM_CLASS; i++) {
		for (j = 0; j < GLCM_CLASS; j++) {
			glcm[i * GLCM_CLASS + j] = 0;
		}
	}

	//计算灰度共生矩阵
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
		//灰度矩阵归一化
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
		//灰度矩阵归一化
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
	//135度对角方向
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
		//灰度矩阵归一化
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
	//45度对角方向
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
		//灰度矩阵归一化
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
	//计算特征值
	double entropy = 0, energy = 0, contrast = 0, homogenity = 0;
	for (i = 0; i < GLCM_CLASS; i++)
	{
		for (j = 0; j < GLCM_CLASS; j++)
		{
			//熵
			if (glcm[i * GLCM_CLASS + j] > 0)
				entropy -= glcm[i * GLCM_CLASS + j] * log10(glcm[i * GLCM_CLASS + j]);
			//能量
			energy += glcm[i * GLCM_CLASS + j] * glcm[i * GLCM_CLASS + j];
			//对比度
			contrast += (i - j) * (i - j) * glcm[i * GLCM_CLASS + j];
			//一致性
			homogenity += 1.0 / (1 + (i - j) * (i - j)) * glcm[i * GLCM_CLASS + j];
		}
	}
	//存储特征值
	GLCMMoment[0] = entropy;
	GLCMMoment[1] = energy;
	GLCMMoment[2] = contrast;
	GLCMMoment[3] = homogenity;
}

void FeatureExtraction::group_extractGLCMFeature() 
{
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double GLCMMoment0[4], GLCMMoment90[4], GLCMMoment45[4], GLCMMoment135[4];
	FILE* fp = fopen(featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个数
		for (int j = begin; j <= end; j++) {//第j个图像
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//第i个数字子目录
			if (i >= 10) {//数不超过99，否则这里会出错
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
			//第i个数字的第j个图片
			if (j >= 10 && j < 100) {//数不超过999，否则这里会出错
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
			get_GLCMMoment(Img, GLCM_ANGLE_0, GLCMMoment0);//计算纹理特征，并赋标签
			get_GLCMMoment(Img, GLCM_ANGLE_45, GLCMMoment45);//计算纹理特征，并赋标签
			get_GLCMMoment(Img, GLCM_ANGLE_90, GLCMMoment90);//计算纹理特征，并赋标签
			get_GLCMMoment(Img, GLCM_ANGLE_135, GLCMMoment135);//计算纹理特征，并赋标签


			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment0[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment45[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment90[d]);
			for (int d = 0; d < 4; d++)
				fprintf(fp, "%f ", GLCMMoment135[d]);
			fprintf(fp, "%d\n", i);//给一个标号
		}
	}
	fclose(fp);
}

void get_GaborFeature(Mat img, double GaborFeature[80])//按格式输入图像
{
	int GaborNum = 40; //Gabor滤波器数量
	int U = 8;   //8个方向
	int V = 5;   //5个尺度
	Mat  GaborReal[40];
	Mat  GaborImg[40];
	getAllGaborKernal(GaborNum, U, V, GaborReal, GaborImg);
	calculate_Feature(img, GaborReal, GaborImg, GaborNum, GaborFeature);
	for (int d = 0; d < 80; d++)
		GaborFeature[d] = GaborFeature[d];
}

void FeatureExtraction::group_extractGaborFeature() {
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double GaborFeature[80];
	FILE* fp = fopen(featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个星
		for (int j = begin; j <= end; j++) {//第j个星
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//第i个人子目录
			if (i >= 10) {//人数不超过99，否则这里会出错
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
			//第i个人的第j个卫星
			if (j >= 10) {//卫星数不超过99，否则这里会出错
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
			get_GaborFeature(Img, GaborFeature);//计算Babor特征，并赋标签
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
	// 将灰度图像转换为归一化的浮点型图像  
	img.convertTo(img, CV_32F);
	normalize(img, img, 0, 1, NORM_MINMAX);
	// 计算均值和标准差  
	Scalar mean;
	Scalar stddev;
	meanStdDev(img, mean, stddev);
	// 第一个特征：灰度图像的均值  
	COLORFeature[0] = mean[0];
	// 第二个特征：灰度图像的标准差  
	COLORFeature[1] = stddev[0];
	// 第三个特征：灰度图像的偏度  
	double skewness = 0.0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			skewness += pow((img.at<float>(i, j) - mean[0]), 3) / (height * width);
		}
	}
	skewness = (skewness < 0) ? -pow(-skewness, 1.0 / 3) : pow(skewness, 1.0 / 3);
	COLORFeature[2] = skewness;
	// 第四个特征：灰度图像的峰度  
	double kurtosis = 0.0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			kurtosis += pow((img.at<float>(i, j) - mean[0]), 4) / (height * width);
		}
	}
	kurtosis = kurtosis / (stddev[0] * stddev[0]) - 3;
	COLORFeature[3] = kurtosis;
	// 第五个特征：灰度直方图的熵  
	double entropy = 0.0;
	Mat hist;
	int histSize = 256; // 直方图的 bin 数量  
	float range[] = { 0, 256 }; // 灰度值范围  
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
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double COLORFeature[3];
	FILE* fp = fopen(featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个数
		for (int j = begin; j <= end; j++) {//第j个图像
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//第i个数字子目录
			if (i >= 10) {//数不超过99，否则这里会出错
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
			//第i个数字的第j个图片
			if (j >= 10 && j < 100) {//数不超过999，否则这里会出错
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
	// 二值化图像
	Mat binaryImage;
	threshold(img, binaryImage, 128, 255, THRESH_BINARY);

	// 计算水平与垂直梯度
	Mat grad_x, grad_y;
	img.convertTo(img, CV_32F);
	normalize(img, img, 0, 1, NORM_MINMAX);
	Sobel(img, grad_x, CV_32F, 1, 0, 1);
	Sobel(img, grad_y, CV_32F, 0, 1, 1);

	// 计算合梯度幅值和方向
	Mat gradientMagnitude, gradientDirection;
	magnitude(grad_x, grad_y, gradientMagnitude);
	phase(grad_x, grad_y, gradientDirection, true);
	normalize(grad_x, grad_x, 0, 1, NORM_MINMAX);
	normalize(grad_y, grad_y, 0, 1, NORM_MINMAX);
	normalize(gradientDirection, gradientDirection, 0, 1, NORM_MINMAX);

	// 形状特征计算
	Scalar mean;
	Scalar sd;

	// 计算水平边缘特征
	meanStdDev(grad_x, mean, sd);
	SHAPEFeature[0] = mean[0];
	SHAPEFeature[1] = sd[0];

	// 计算垂直边缘特征
	meanStdDev(grad_y, mean, sd);
	SHAPEFeature[2] = mean[0];
	SHAPEFeature[3] = sd[0];

	// 计算角度特征
	meanStdDev(gradientDirection, mean, sd);
	SHAPEFeature[4] = mean[0];
	SHAPEFeature[5] = sd[0];
}

void FeatureExtraction::group_extractSHAPEFeature()
{
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double SHAPEFeature[3];
	FILE* fp = fopen(featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个数
		for (int j = begin; j <= end; j++) {//第j个图像
			char filename[200], s[100];
			strcpy(filename, bmpFileRoot);
			strcat(filename, "s");
			//第i个数字子目录
			if (i >= 10) {//数不超过99，否则这里会出错
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
			//第i个数字的第j个图片
			if (j >= 10 && j < 100) {//数不超过999，否则这里会出错
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
			//cout << filename << endl; D:/VS code/projects/模式识别实验/实验一/卫星库/s16/4.bmp

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