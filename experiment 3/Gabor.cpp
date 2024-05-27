#include "Gabor.h"

/*
*@函数名： conv2
*@功能：   卷积运算
*@参数：   const Mat &img    原图像
const Mat ikernel 卷积核
convtype type     卷积类型
*@返回值： Mat       矩阵卷积后的结果
*/
Mat conv2(const Mat &img, const Mat ikernel)
{
	Mat dst(img.rows, img.cols, CV_32F);
	Mat kernel;
	flip(ikernel, kernel, -1);//将卷积核沿x,y轴翻转
	filter2D(img, dst, img.depth(), kernel);
	return dst;
}

/*
*@函数名： MakeAllGaborKernal
*@功能：   生成所有Gabor核
*/
void getAllGaborKernal(int GaborNum, int U, int V, Mat kel_GR[], Mat kel_GI[])
{
	int n = 0;
	int kernel_w = 17;
	int kernel_h = 17;

	for (int v = 0;v < V;v++)
	{
		for (int u = 0;u < U;u++)
		{
			makeGaborKernal(kernel_w, kernel_h, u, v, kel_GR[v * 8 + u], kel_GI[v * 8 + u]);
		}
	}
}

/*
*@函数名： MakeGaborKernal
*@功能：   生成固定方向与固定尺度的Gabor核
*@参数：   int ke_h            核函数的高度
int ke_w            核函数的宽度
int u               Gabor的尺度
int v               Gabor的方向
Mat &GaborReal      Gabor的实部
Mat &GaborImg       Gabor的虚部
*@返回值： Mat              返回的Gabor特征
*/
void makeGaborKernal(int ke_h, int ke_w, int u, int v, Mat &GaborReal, Mat &GaborImg)
{
	int HalfH = ke_h / 2;
	int HalfW = ke_w / 2;
	double m_kmax = CV_PI / 2;
	double m_f = sqrt(2.0);
	double m_sigma = CV_PI;
	double Qu = CV_PI*u / 8;
	double sqsigma = m_sigma*m_sigma;
	double Kv = m_kmax / (pow(m_f, v));
	double postmean = exp(-sqsigma / 2);

	GaborReal.create(HalfH + HalfW + 1, HalfH + HalfW + 1, CV_32FC1);
	GaborImg.create(HalfH + HalfW + 1, HalfH + HalfW + 1, CV_32FC1);
	float *ptr_real = NULL;
	float *ptr_img = NULL;

	for (int j = -HalfH;j <= HalfH;j++)
	{
		ptr_real = GaborReal.ptr<float>(j + HalfH);
		ptr_img = GaborImg.ptr<float>(j + HalfH);

		for (int i = -HalfW;i <= HalfW;i++)
		{
			double tmp1 = exp(-(Kv*Kv*(j*j + i*i) / (2 * sqsigma)));
			double tmp2 = cos(Kv*cos(Qu)*i + Kv*sin(Qu)*j) - postmean;
			double tmp3 = sin(Kv*cos(Qu)*i + Kv*sin(Qu)*j);
			ptr_real[i + HalfW] = Kv*Kv*tmp1*tmp2 / sqsigma;
			ptr_img[i + HalfW] = Kv*Kv*tmp1*tmp3 / sqsigma;
		}
	}
}

/*
*@函数名： calculate_Feature
*@功能：   获取固定核函数的Gabor均值与方差二维特征
*@参数：   Mat &src                原图---彩色图像
Mat kel_GR[Gabor_num]   固定核函数的实部
Mat kel_GI[Gabor_num]   固定核函数的虚部
double* feature         特征值
*/
void calculate_Feature(Mat &src, Mat kel_GR[], Mat kel_GI[], int GaborNum, double feature[])
{
	vector<float> featureTmp;
	for (int i = 0;i<GaborNum;i++)
	{
		Mat feat_real_part(src.rows, src.cols, CV_32F);
		Mat feat_img_part(src.rows, src.cols, CV_32F);
		Mat feat_mode_part(src.rows, src.cols, CV_32F);

		feat_real_part = conv2(src, kel_GR[i]);
		feat_img_part = conv2(src, kel_GI[i]);
		multiply(feat_real_part, feat_real_part, feat_real_part);
		multiply(feat_img_part, feat_img_part, feat_img_part);
		add(feat_real_part, feat_img_part, feat_mode_part);
		feat_mode_part.convertTo(feat_mode_part, CV_32F);

		sqrt(feat_mode_part, feat_mode_part);
		Mat mean;
		Mat stddev;
		meanStdDev(feat_mode_part, mean, stddev);

		feature[i * 2] = mean.at<double>(0, 0);
		feature[i * 2 + 1] = stddev.at<double>(0, 0);
	}
}
