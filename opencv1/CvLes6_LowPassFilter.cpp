#include"pch.h"
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

//定义全局变量
Mat g_srcImage;         //定义输入图像
Mat g_dstImage;         //定义目标图像
Mat g_nosieImage;         //定义目标图像

const int g_nTrackbarMaxValue = 9;      //定义轨迹条最大值
int g_nTrackbarValue;                   //定义轨迹条初始值
int g_nKernelValue;                     //定义kernel尺寸

void on_kernelTrackbar(int, void*);     //定义回调函数
Mat addnoise(Mat img, float SNR);

int main()
{
	Mat g_grayImage;
	g_srcImage = imread("Lenna.jpg");

	//判断图像是否加载成功
	if (g_srcImage.empty())
	{
		cout << "图像加载失败!" << endl;
		return -1;
	}
	else
		cout << "图像加载成功!" << endl << endl;

	cvtColor(g_srcImage, g_grayImage, COLOR_BGR2GRAY);
	imshow("g_grayImage", g_grayImage);

	g_nosieImage = addnoise(g_grayImage, 0.95);
	imshow("g_nosieImage", g_nosieImage);

	g_nTrackbarValue = 1;
	namedWindow("均值滤波", WINDOW_AUTOSIZE);   //定义滤波后图像显示窗口属性

	//定义轨迹条名称和最大值
	char kernelName[20];
	sprintf_s(kernelName, "kernel尺寸 %d", g_nTrackbarMaxValue);

	//创建轨迹条
	createTrackbar(kernelName, "均值滤波", &g_nTrackbarValue, g_nTrackbarMaxValue, on_kernelTrackbar);
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
	//根据输入值重新计算kernel尺寸
	g_nKernelValue = g_nTrackbarValue * 2 + 1;

	//均值滤波函数
	blur(g_nosieImage, g_dstImage, Size(g_nKernelValue, g_nKernelValue));

	imshow("均值滤波", g_dstImage);
}

void edge_preserving(Mat img)
{
	Mat out;
	img.copyTo(out);
	int row = img.rows;
	int col = img.cols;
	



}




