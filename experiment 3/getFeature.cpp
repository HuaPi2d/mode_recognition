#include "getFeature.h"
#include "Gabor.h"

#define Color_F_N 32
//��ȡ��������������Ҷȹ�������4������
#define GLCM_DIS 1 //�Ҷȹ��������ͳ�ƾ���
#define GLCM_CLASS 16 //�Ҷȹ�������ͼ��Ҷ�ֵ�ȼ���
#define GLCM_ANGLE_0 0//ˮƽ
#define GLCM_ANGLE_90 1//��ֱ
#define GLCM_ANGLE_45 45//45�ȶԽ�
#define GLCM_ANGLE_135 135//135�ȶԽ�

void get_GLCMMoment(Mat img, int angle, double GLCMMoment[5]) {
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
	// cout << entropy << '\n' << energy << '\n' << contrast << '\n' << homogenity << '\n' << endl;
	//�洢����ֵ
	GLCMMoment[0] = entropy;
	GLCMMoment[1] = energy;
	GLCMMoment[2] = contrast;
	GLCMMoment[3] = homogenity;
}

void group_extractGLCMFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile)
{
	FILE* fp;
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double GLCMMoment0[4], GLCMMoment90[4], GLCMMoment45[4], GLCMMoment135[4];
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j��ͼ��
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");
			// cout << filename << endl; //D:/VS code/projects/ģʽʶ��ʵ��/ʵ��һ/���ǿ�/s16/4.bmp

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

void group_extractGaborFeature(char* bmpFileRoot, int classNum, int begin, int end, char* featureOutputFile) {
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double GaborFeature[80];
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j����
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			get_GaborFeature(Img, GaborFeature);//����Babor������������ǩ
			for (int d = 0; d < 80; d++)
				fprintf(fp, "%f ", GaborFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

//��ȡ��ɫ����
void get_ColorFeature(Mat img, double ColorFeature[Color_F_N])
{
	const int channels[] = { 0 };
	Mat hist;//�������Mat����
	int dims = 1;//����ֱ��ͼά��
	const int histSize =  Color_F_N ; //ֱ��ͼÿһ��ά�Ȼ��ֵ���������Ŀ
	float pranges[] = { 0, 255 };//ȡֵ����
	const float* ranges[] = { pranges };

	calcHist(&img, 1, channels, Mat(), hist, dims, &histSize, ranges, true, false);//����ֱ��ͼ
	for (int i = 0; i < Color_F_N; i++)
	{
		ColorFeature[i] = hist.at<float>(i);   //����histԪ�أ�ע��hist����float���ͣ�
	}
	return;
	
}

void group_extractColorFeature(const char* bmpFileRoot, int classNum, int begin, int end, const char* featureOutputFile)
{
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	double ColorFeature[Color_F_N];
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j����
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			get_ColorFeature(Img, ColorFeature);//����Babor������������ǩ
			for (int d = 0; d < Color_F_N; d++)
				fprintf(fp, "%f ", ColorFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

//HOG������ȡ
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
	Mat Img;//Matͼ���࣬ Imgͼ���һ�������൱��һ��ͼ�����
	std::vector<float> HOGFeature;
	FILE* fp;
	errno_t err = fopen_s(&fp, featureOutputFile, "w");//ѵ��������
	for (int i = 1; i <= classNum; i++) {//��i����
		for (int j = begin; j <= end; j++) {//��j����
			char filename[200], s[100];
			strcpy_s(filename, bmpFileRoot);
			strcat_s(filename, "s");
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
			strcat_s(filename, s);
			strcat_s(filename, "/");
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
			strcat_s(filename, s);
			strcat_s(filename, ".bmp");

			Img = imread(filename);
			HOGFeature = get_HOGFeature(Img);//������״������������ǩ
			for (int d = 0; d < HOGFeature.size(); d++)
				fprintf(fp, "%f ", HOGFeature[d]);
			fprintf(fp, "%d\n", i);
		}
	}
	fclose(fp);
}

