#include "Gabor.h"

/*
*@�������� conv2
*@���ܣ�   �������
*@������   const Mat &img    ԭͼ��
const Mat ikernel �����
convtype type     �������
*@����ֵ�� Mat       ��������Ľ��
*/
Mat conv2(const Mat &img, const Mat ikernel)
{
	Mat dst(img.rows, img.cols, CV_32F);
	Mat kernel;
	flip(ikernel, kernel, -1);//���������x,y�ᷭת
	filter2D(img, dst, img.depth(), kernel);
	return dst;
}

/*
*@�������� MakeAllGaborKernal
*@���ܣ�   ��������Gabor��
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
*@�������� MakeGaborKernal
*@���ܣ�   ���ɹ̶�������̶��߶ȵ�Gabor��
*@������   int ke_h            �˺����ĸ߶�
int ke_w            �˺����Ŀ��
int u               Gabor�ĳ߶�
int v               Gabor�ķ���
Mat &GaborReal      Gabor��ʵ��
Mat &GaborImg       Gabor���鲿
*@����ֵ�� Mat              ���ص�Gabor����
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
*@�������� calculate_Feature
*@���ܣ�   ��ȡ�̶��˺�����Gabor��ֵ�뷽���ά����
*@������   Mat &src                ԭͼ---��ɫͼ��
Mat kel_GR[Gabor_num]   �̶��˺�����ʵ��
Mat kel_GI[Gabor_num]   �̶��˺������鲿
double* feature         ����ֵ
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
