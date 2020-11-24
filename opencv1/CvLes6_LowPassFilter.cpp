#include"pch.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

//����ȫ�ֱ���
Mat g_srcImage;         //��������ͼ��
Mat g_dstImage;         //����Ŀ��ͼ��
Mat g_nosieImage;         //����Ŀ��ͼ��

const int g_nTrackbarMaxValue = 9;      //����켣�����ֵ
int g_nTrackbarValue;                   //����켣����ʼֵ
int g_nKernelValue;                     //����kernel�ߴ�

void on_kernelTrackbar(int, void*);     //����ص�����
Mat addnoise(Mat img, float SNR);

int main()
{
	Mat g_grayImage;
	g_srcImage = imread("Lenna.jpg");

	//�ж�ͼ���Ƿ���سɹ�
	if (g_srcImage.empty())
	{
		cout << "ͼ�����ʧ��!" << endl;
		return -1;
	}
	else
		cout << "ͼ����سɹ�!" << endl << endl;

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	imshow("g_grayImage", g_grayImage);

	g_nosieImage = addnoise(g_grayImage, 0.95);
	imshow("g_nosieImage", g_nosieImage);

	g_nTrackbarValue = 1;
	namedWindow("��ֵ�˲�", WINDOW_AUTOSIZE);   //�����˲���ͼ����ʾ��������

	//����켣�����ƺ����ֵ
	char kernelName[20];
	sprintf_s(kernelName, "kernel�ߴ� %d", g_nTrackbarMaxValue);

	//�����켣��
	createTrackbar(kernelName, "��ֵ�˲�", &g_nTrackbarValue, g_nTrackbarMaxValue, on_kernelTrackbar);
	on_kernelTrackbar(g_nTrackbarValue, 0);

	
	/*k=cv::waitKey(5) & 0xff;
	if (k == 27)
		break;*/

	waitKey(9000);

	return 0;
}

Mat addnoise(Mat img, float SNR)
{
	Mat out;
	img.copyTo(out);
	int row = img.rows;
	int col = img.cols;
	int NP, NP_x, NP_y;
	NP = (int)(row * col*(1 - SNR));
	srand((unsigned)time(NULL));
	for (int i = 0; i < NP; i++)
	{
		NP_x = rand() % row;
		NP_y = rand() % col;
		if (rand() % 2 == 0)
			out.row(NP_x).col(NP_y) = 255;
		else
			out.row(NP_x).col(NP_y) = 0;
	}
	return out;
}

void on_kernelTrackbar(int, void*)
{
	//��������ֵ���¼���kernel�ߴ�
	g_nKernelValue = g_nTrackbarValue * 2 + 1;

	//��ֵ�˲�����
	blur(g_nosieImage, g_dstImage, Size(g_nKernelValue, g_nKernelValue));

	imshow("��ֵ�˲�", g_dstImage);
}

void edge_preserving(Mat img)
{
	Mat out;
	img.copyTo(out);
	int row = img.rows;
	int col = img.cols;
	



}




