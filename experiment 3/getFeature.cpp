#include "getFeature.h"
#include "Gabor.h"

#define Color_F_N 32
//提取纹理特征，计算灰度共生矩阵，4个特征
#define GLCM_DIS 1 //灰度共生矩阵的统计距离
#define GLCM_CLASS 16 //灰度共生矩阵图像灰度值等级化
#define GLCM_ANGLE_0 0//水平
#define GLCM_ANGLE_90 1//垂直
#define GLCM_ANGLE_45 45//45度对角
#define GLCM_ANGLE_135 135//135度对角

void get_GLCMMoment(Mat img, int angle, double GLCMMoment[5]) {
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
	// cout << entropy << '\n' << energy << '\n' << contrast << '\n' << homogenity << '\n' << endl;
	//存储特征值
	GLCMMoment[0] = entropy;
	GLCMMoment[1] = energy;
	GLCMMoment[2] = contrast;
	GLCMMoment[3] = homogenity;
}

void group_extractGLCMFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile)
{
	FILE* fp;
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double GLCMMoment0[4], GLCMMoment90[4], GLCMMoment45[4], GLCMMoment135[4];
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个数
		for (int j = begin; j <= end; j++) {//第j个图像
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");
			// cout << filename << endl; //D:/VS code/projects/模式识别实验/实验一/卫星库/s16/4.bmp

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

void group_extractGaborFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile) {
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double GaborFeature[80];
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个星
		for (int j = begin; j <= end; j++) {//第j个星
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			get_GaborFeature(Img, GaborFeature);//计算Babor特征，并赋标签
			for (int d = 0; d < 80; d++)
				fprintf(fp, "%f ", GaborFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

//提取颜色特征
void get_ColorFeature(Mat img, double ColorFeature[Color_F_N])
{
	const int channels[] = { 0 };
	Mat hist;//定义输出Mat类型
	int dims = 1;//设置直方图维度
	const int histSize =  Color_F_N ; //直方图每一个维度划分的柱条的数目
	float pranges[] = { 0, 255 };//取值区间
	const float* ranges[] = { pranges };

	calcHist(&img, 1, channels, Mat(), hist, dims, &histSize, ranges, true, false);//计算直方图
	for (int i = 0; i < Color_F_N; i++)
	{
		ColorFeature[i] = hist.at<float>(i);   //遍历hist元素（注意hist中是float类型）
	}
	return;
	
}

void group_extractColorFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile)
{
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	double ColorFeature[Color_F_N];
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个星
		for (int j = begin; j <= end; j++) {//第j个星
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			get_ColorFeature(Img, ColorFeature);//计算Babor特征，并赋标签
			for (int d = 0; d < Color_F_N; d++)
				fprintf(fp, "%f ", ColorFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

//HOG特征提取
std::vector<float> get_HOGFeature(Mat img) 
{
	std::vector<float> HOGFeature;
	cv::HOGDescriptor hog;

	hog.winSize = cv::Size(img.cols, img.rows);
	hog.blockSize = cv::Size(img.cols / 4, img.rows / 4);
	hog.blockStride = cv::Size(img.cols / 4, img.rows / 4);
	hog.cellSize = cv::Size(img.cols / 4, img.rows / 4);
	hog.compute(img, HOGFeature);

	return HOGFeature;
}

void group_extractHOGFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile) {
	Mat Img;//Mat图像类， Img图像的一个对象，相当于一个图像变量
	std::vector<float> HOGFeature;
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//训练集数据
	for (int i = 1; i <= classNum; i++) {//第i个星
		for (int j = begin; j <= end; j++) {//第j个星
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			HOGFeature = get_HOGFeature(Img);//计算形状特征，并赋标签
			for (int d = 0; d < HOGFeature.size(); d++)
				fprintf(fp, "%f ", HOGFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

