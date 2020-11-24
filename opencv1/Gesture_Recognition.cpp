#include "pch.h"
/*----------------------------------������˵����---------------------------------------|
|	�������ƣ������Ӿ�������ʶ�𽻻�ϵͳ											   |
|	�����ܣ������ƴ����������˻�����											   |
|	����ʱ�䣺														           |
|	�������ߣ�														           |
|-------------------------------------------------------------------------------------*/

/*----------------------------------��ͷ�ļ��������ռ�������֡�-----------------------|
|	����������������ʹ�õ�ͷ�ļ��������ռ�										       |
|-------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <stack>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/types_c.h>
#include <Windows.h>
#include<time.h>
using namespace std;
using namespace cv;

/*-----------------------------------�����������������֡�-------------------------------|
|		������������������																|
|--------------------------------------------------------------------------------------*/
// ��ʾ��Ϣ
void ShowHelpText()
{
	//�����ӭ��Ϣ��OpenCV�汾
	cout << "\n      �������Ӿ�������ʶ�𽻻�ϵͳ��ơ�";
	cout << "\nDesign of vision-based gesture recognition interactive system";
	cout << "\n���ߣ��ƿ�	���ܣ������ƴ����������˻�����";
	cout << "\n---------------------------------------------------";
	cout << "\n����1������ƶ�";
	cout << "\n����2������������  ����3����������Ҽ�";
	cout << "\n����4������������  ����5���ɿ�������";
	cout << "\n---------------------------------------------------\n";

}

/*-----------------------------------�����������������֡�-------------------------------|
|		������������������																|
|--------------------------------------------------------------------------------------*/
// ��ʾ��Ϣ
void ShowDeBugInfo()
{
	//�����ӭ��Ϣ��OpenCV�汾
	cout << "\n  ";
	cout << "\n---------------------------------------------------\n";

}

// ���ڽ������㷨��������ÿ������ı�Ե��
void Seed_Filling(const cv::Mat& binImg, cv::Mat& labelImg, int& labelNum, int(&ymin)[20], int(&ymax)[20], int(&xmin)[20], int(&xmax)[20])  //������䷨ 
{
	if (binImg.empty() ||
		binImg.type() != CV_8UC1)// ���ͼ���ǿջ��߸�ʽ����ȷ�ͷ���
	{
		return;
	}

	labelImg.release();
	binImg.convertTo(labelImg, CV_32SC1);// ������������ת��
	int label = 0;
	int rows = binImg.rows;
	int cols = binImg.cols;
	for (int i = 1; i < rows - 1; i++)
	{
		int* data = labelImg.ptr<int>(i);
		for (int j = 1; j < cols - 1; j++)
		{

			if (data[j] == 0)
			{
				std::stack<std::pair<int, int>> neighborPixels;// std::pair��Ҫ�������ǽ�����������ϳ�һ�����ݣ��������ݿ�����ͬһ���ͻ��߲�ͬ���͡�
															   // pair��һ��ģ��ṹ�壬����Ҫ��������Ա������first��second����������������ֱ��ʹ�á�
				neighborPixels.push(std::pair<int, int>(j, i));// ��ջ������Ԫ������λ��: <j,i>
				ymin[label] = i;
				ymax[label] = i;
				xmin[label] = j;
				xmax[label] = j;
				while (!neighborPixels.empty())
				{
					std::pair<int, int> curPixel = neighborPixels.top();// �������һ����һ�������غ���������һ�е��Ǹ��ŵı�Ÿ����� 
					int curX = curPixel.first;
					int curY = curPixel.second;
					labelImg.at<int>(curY, curX) = 255;
					neighborPixels.pop();	// ��ջ

					if ((curX > 0) && (curY > 0) && (curX < (cols - 1)) && (curY < (rows - 1)))
					{
						if (labelImg.at<int>(curY - 1, curX) == 0)                      //��
						{
							neighborPixels.push(std::pair<int, int>(curX, curY - 1));
							//ymin[label] = curY - 1;
						}

						if (labelImg.at<int>(curY + 1, curX) == 0)                      //��
						{
							neighborPixels.push(std::pair<int, int>(curX, curY + 1));
							if ((curY + 1) > ymax[label])
								ymax[label] = curY + 1;
						}

						if (labelImg.at<int>(curY, curX - 1) == 0)                      //��
						{
							neighborPixels.push(std::pair<int, int>(curX - 1, curY));
							if ((curX - 1) < xmin[label])
								xmin[label] = curX - 1;
						}

						if (labelImg.at<int>(curY, curX + 1) == 0)                      //��
						{
							neighborPixels.push(std::pair<int, int>(curX + 1, curY));
							if ((curX + 1) > xmax[label])
								xmax[label] = curX + 1;
						}

						if (labelImg.at<int>(curY - 1, curX - 1) == 0)                  //����
						{
							neighborPixels.push(std::pair<int, int>(curX - 1, curY - 1));
							//ymin[label] = curY - 1;
							if ((curX - 1) < xmin[label])
								xmin[label] = curX - 1;
						}
						if (labelImg.at<int>(curY + 1, curX + 1) == 0)                  //����
						{
							neighborPixels.push(std::pair<int, int>(curX + 1, curY + 1));
							if ((curY + 1) > ymax[label])
								ymax[label] = curY + 1;
							if ((curX + 1) > xmax[label])
								xmax[label] = curX + 1;
						}

						if (labelImg.at<int>(curY + 1, curX - 1) == 0)                  //����
						{
							neighborPixels.push(std::pair<int, int>(curX - 1, curY + 1));
							if ((curY + 1) > ymax[label])
								ymax[label] = curY + 1;
							if ((curX - 1) < xmin[label])
								xmin[label] = curX - 1;
						}

						if (labelImg.at<int>(curY - 1, curX + 1) == 0)                  //����
						{
							neighborPixels.push(std::pair<int, int>(curX + 1, curY - 1));
							//ymin[label] = curY - 1;
							if ((curX + 1) > xmax[label])
								xmax[label] = curX + 1;
						}
					}
				}
				++label; // û���ظ����ţ���ʼ�µı�ǩ
			}
		}
	}
	labelNum = label;
}

// ����
void mirrorY(Mat src, Mat &dst)
{
	int row = src.rows;
	int col = src.cols;
	dst = src.clone();
	for (int i = 0; i < col; i++) {
		src.col(col - 1 - i).copyTo(dst.col(i));
	}
}

// ��ˮ���㷨
class WatershedSegmenter {
private:
	cv::Mat markers;
public:
	void setMarkers(const cv::Mat& markerImage) {

		// ת��Ϊ����ͼ��
		markerImage.convertTo(markers, CV_32S);
	}

	cv::Mat process(const cv::Mat &image) {
		// ���÷�ˮ��
		cv::watershed(image, markers);
		return markers;
	}

	// ��ͼ�����ʽ���ؽ��
	cv::Mat getSegmentation() {
		cv::Mat tmp;
		// ��ǩ����255�����ж�
		// ������ֵΪ255
		markers.convertTo(tmp, CV_8U);
		return tmp;
	}

	// ��ͼ�����ʽ���ط�ˮ��
	cv::Mat getWatersheds() {
		cv::Mat tmp;
		markers.convertTo(tmp, CV_8U, 255, 255);
		return tmp;
	}
};
/*--------------------------------------��main()������-----------------------------------|
|		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼִ��						 |
|---------------------------------------------------------------------------------------*/
int main_inst()
{
	// ��ʾ������Ϣ
	ShowHelpText();

	// ������Ƶ���룬�������������������ͷ��ѡ��һ���Դ�����0
	cv::VideoCapture cap(0);

	if (!cap.isOpened())
	{
		return -1;
	}

	clock_t start, finish;
	double totaltime;

	string str1, str2, str3;
	Mat frame;
	Mat binImage, tmp, tmp1;
	Mat Y, Cr, Cb, H;
	vector<Mat> channels, channels1;

	//ģ��ͼƬ
	Mat mu[5];
	mu[0] = imread("m1.png", CV_8UC1); // ����1
	mu[1] = imread("m2.png", CV_8UC1); // ����2
	mu[2] = imread("m3.png", CV_8UC1); // ����3
	mu[3] = imread("m4.png", CV_8UC1); // ����4
	mu[4] = imread("m5.png", CV_8UC1); // ����5
	float simi[5];	// ģ��ƥ��
	float flag;		// ��־
	int flagx = 0, flagx1 = 0, flagx2 = 0;		// �м����
	float k = 1.5;	// �����������
	int curpointx = 0, curpointy = 0, prepointx = 367, prepointy = 80;	// ����ʼλ��
	int xg_num = 0;

	bool stop = false;
	while (!stop)
	{
		cap >> frame;
		//imshow("The original image", frame);		// ������ʾ��ԭʼͼ��
		//start = clock();


		// ����
		mirrorY(frame, frame);

		// ��ɫ�ռ�任��RGB to GRAY��
		cvtColor(frame, binImage, CV_BGR2GRAY);

		// ��ɫ�ռ�任��RGB to YCrCb��
		frame.copyTo(tmp);
		cvtColor(tmp, tmp, CV_BGR2YCrCb);

		// ��ɫ�ռ�任��RGB to HSV��
		frame.copyTo(tmp1);
		cvtColor(tmp1, tmp1, CV_BGR2HSV);
	
		// ͨ������
		split(tmp, channels);
		split(tmp1, channels1);
		Cr = channels.at(1);	// �������ɫ��Cr��
		Cb = channels.at(2);	// ����������Ͷ�Cb��
		H = channels1.at(0);	// �������H��
		
		// ��ɫ��⣬�����ֵͼ��
		for (int j = 1; j < Cr.rows - 1; j++)	// ����ͼ�����ص�
		{
			uchar* currentCr = Cr.ptr< uchar>(j);
			uchar* currentCb = Cb.ptr< uchar>(j);
			uchar* currentH = H.ptr< uchar>(j);
			uchar* current = binImage.ptr< uchar>(j);

			for (int i = 1; i < Cb.cols - 1; i++)
			{
				if ((currentCr[i] >= 135) && (currentCr[i] <= 170) && (currentCb[i] >= 94) && (currentCb[i] <= 125) && (currentH[i] >= 1) && (currentH[i] <= 23))
					current[i] = 255;
				else
					current[i] = 0;
			}
		}

		// �Ҷ���̬ѧ����
		erode(binImage, binImage, Mat());
		dilate(binImage, binImage, Mat());

		// ���ڱ�ǵķ�ˮ���㷨
		cv::Mat fg;
		cv::erode(binImage, fg, cv::Mat(), cv::Point(-1, -1), 6);	// ���εݹ鸯ʴ

		// ʶ��û�ж����ͼ������
		cv::Mat bg;
		cv::dilate(binImage, bg, cv::Mat(), cv::Point(-1, -1), 6);	// ���εݹ�����
		cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);	// ��������ֵ����ͼ��ָ�cv::THRESH_BINARY_INV ������ֵ �� ֵ��Ϊ 0,����Ϊ 128 �ڰ׶�ֵ��ת(��ת��ֵ��ֵ��)

		// ��ʾ���ͼ��
		cv::Mat markers(binImage.size(), CV_8U, cv::Scalar(0));
		markers = fg + bg;

		// ������ˮ��ָ����
		WatershedSegmenter segmenter;
		segmenter.setMarkers(markers);
		segmenter.process(frame);// Ӧ�÷�ˮ���㷨

		Mat waterShed;
		waterShed = segmenter.getSegmentation();

		// 8�������㷨�����߿������
		Mat labelImg;
		int label, ymin[20], ymax[20], xmin[20], xmax[20];
		Seed_Filling(waterShed, labelImg, label, ymin, ymax, xmin, xmax);

		// ͳ��һ�������еķ�ɫ�������
		float fuseratio[20];
		for (int k = 0; k < label; k++)
		{
			fuseratio[k] = 1;
			if (((xmax[k] - xmin[k] + 1) > 50) && ((xmax[k] - xmin[k] + 1) < 300) && ((ymax[k] - ymin[k] + 1) > 150) && ((ymax[k] - ymin[k] + 1) < 450))
			{
				int fusepoint = 0;
				for (int j = ymin[k]; j < ymax[k]; j++)
				{
					uchar* current = waterShed.ptr< uchar>(j);
					for (int i = xmin[k]; i < xmax[k]; i++)
					{
						if (current[i] == 255)
							fusepoint++;
					}
				}
				fuseratio[k] = float(fusepoint) / ((xmax[k] - xmin[k] + 1) * (ymax[k] - ymin[k] + 1));
			}
		}

		Size dsize = Size(108, 128);
		// ��������ֵ������λ�û���
		for (int i = 0; i < label; i++)
		{
			if ((fuseratio[i] < 0.58))
			{
				// �߶ȵ���
				Mat rROI = Mat(dsize, CV_8UC1);
				resize(waterShed(Rect(xmin[i], ymin[i], xmax[i] - xmin[i], ymax[i] - ymin[i])), rROI, dsize);
				imshow("��������", rROI);

				// ģ��ƥ��
				Mat result;
				for (int mp = 0; mp < 5; mp++)
				{
					matchTemplate(rROI, mu[mp], result, CV_TM_SQDIFF_NORMED);
					simi[mp] = result.ptr<float>(0)[0];
				}

				// Ѱ�����ƥ��
				flagx2 = flagx1;
				flagx1 = flagx;
				flagx = 0;
				flag = simi[0];
				for (int j = 1; j < 5; j++)
				{
					if (simi[j] < flag)
					{
						flagx = j;
						flag = simi[j];
					}
				}

				cv::Point end = cv::Point(xmin[i], ymin[i] - 15);   // �ӱ�ǩ��λ�á�
				str1 = "(" + to_string(xmin[i]) + "," + to_string(ymin[i]) + ") " + " (" + to_string(xmax[i]) + "," + to_string(ymax[i]) + ")";
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
				putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(xmin[i] - 80, ymin[i] + 20);   // �ӱ�ǩ���ֲ���ǡ�
				str1 = "Hand" + to_string(flagx + 1);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 20);   // �ӱ�ǩ����ɫ������
				str1 = "Skin area ratio: " + to_string(fuseratio[i]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 40);   // �ӱ�ǩ������
				str1 = "Height: " + to_string(ymax[i] - ymin[i]) + "  Width: " + to_string(xmax[i] - xmin[i]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 60);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_1: " + to_string(simi[0]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 80);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_2: " + to_string(simi[1]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 100);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_3: " + to_string(simi[2]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);


				end = cv::Point(20, 120);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_4: " + to_string(simi[3]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 140);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_5: " + to_string(simi[4]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				if (flagx == 0)
				{
					curpointx = xmin[i];
					curpointy = ymin[i];

					int dx = curpointx - prepointx;
					int dy = curpointy - prepointy;

					prepointx = curpointx;
					prepointy = curpointy;
					mouse_event(MOUSEEVENTF_MOVE, k * dx, k * dy, 0, 0);	// ����ƶ�

					end = cv::Point(20, 400);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_MOVE";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}

				if (flagx == 1 && flagx1 != 1 && flagx2 != 1)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_CLICK";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 2 && flagx1 != 2 && flagx2 != 2)
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);			// ����Ҽ�����
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);			// ����Ҽ��ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_RIGHT_CLICK";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 3)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_DOWN";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 4)
				{
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_UP";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}

				rectangle(waterShed, Point(xmin[i], ymin[i]), Point(xmax[i], ymax[i]), Scalar::all(255), 3, 8, 0);	// �ӿ�
				rectangle(frame, Point(xmin[i], ymin[i]), Point(xmax[i], ymax[i]), cv::Scalar(0, 255, 0), 3, 8, 0);	// �ӿ�
			}
		}
		imshow("��Ϣ��ʾ", waterShed);
		imshow("������ʾ", frame);

		// ʱ����ʾ
		//finish = clock();
		//totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
		//cout << "\n�˳��������ʱ��Ϊ" << totaltime << "�룡" << endl;

		if (waitKey(1) >= 0)
			stop = true;

	}

	return 0;
}




int main_inst2()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened())
	{	
		cout << "can't open video\n";
		return -1;
	}

	string str1, str2, str3;
	Mat frame, binImage, frame_HSV, frame_YCrCb;
	Mat Cr, Cb, H;
	vector<Mat> channels, channels1;

	float simi[5];	// ģ��ƥ��
	float flag;		// ��־
	int flagx = 0, flagx1 = 0, flagx2 = 0;		// �м����
	float k = 1.5;	// �����������
	int curpointx = 0, curpointy = 0, prepointx = 367, prepointy = 80;	// ����ʼλ��
	int xg_num = 0;

	//ģ��ͼƬ
	Mat mu[5];
	mu[0] = imread("m1.png", CV_8UC1); // ����1
	mu[1] = imread("m2.png", CV_8UC1); // ����2
	mu[2] = imread("m3.png", CV_8UC1); // ����3
	mu[3] = imread("m4.png", CV_8UC1); // ����4
	mu[4] = imread("m5.png", CV_8UC1); // ����5

	bool stop = false;
	while (!stop)
	{
		cap >> frame;
		mirrorY(frame, frame);
		//imshow("frame_mirror", frame);

		cvtColor(frame, binImage, CV_BGR2GRAY);

		frame.copyTo(frame_YCrCb);
		cvtColor(frame_YCrCb, frame_YCrCb, CV_BGR2YCrCb);
		frame.copyTo(frame_HSV);
		cvtColor(frame_HSV, frame_HSV, CV_BGR2HSV);

		split(frame_YCrCb, channels);
		split(frame_HSV, channels1);

		Cr = channels.at(1);	// �������ɫ��Cr��
		Cb = channels.at(2);	// ����������Ͷ�Cb��
		H = channels1.at(0);	// �������H��

		//imshow("Cr", Cr);
		//imshow("Cb", Cb);
		//imshow("H", H);

		erode(binImage, binImage, Mat());
		//imshow("erode", binImage);
		dilate(binImage, binImage, Mat());
		//imshow("dilate", binImage);

		// ���ڱ�ǵķ�ˮ���㷨
		cv::Mat fg;
		cv::erode(binImage, fg, cv::Mat(), cv::Point(-1, -1), 6);	// ���εݹ鸯ʴ

		// ʶ��û�ж����ͼ������
		cv::Mat bg;
		cv::dilate(binImage, bg, cv::Mat(), cv::Point(-1, -1), 6);	// ���εݹ�����
		cv::threshold(bg, bg, 1, 128, cv::THRESH_BINARY_INV);	// ��������ֵ����ͼ��ָ�cv::THRESH_BINARY_INV ������ֵ �� ֵ��Ϊ 0,����Ϊ 128 �ڰ׶�ֵ��ת(��ת��ֵ��ֵ��)

		// ��ʾ���ͼ��
		cv::Mat markers(binImage.size(), CV_8U, cv::Scalar(0));
		markers = fg + bg;

		// ������ˮ��ָ����
		WatershedSegmenter segmenter;
		segmenter.setMarkers(markers);
		segmenter.process(frame);// Ӧ�÷�ˮ���㷨

		Mat waterShed;
		waterShed = segmenter.getSegmentation();

		// 8�������㷨�����߿������
		Mat labelImg;
		int label, ymin[20], ymax[20], xmin[20], xmax[20];
		Seed_Filling(waterShed, labelImg, label, ymin, ymax, xmin, xmax);

		// ͳ��һ�������еķ�ɫ�������
		float fuseratio[20];
		for (int k = 0; k < label; k++)
		{
			fuseratio[k] = 1;
			if (((xmax[k] - xmin[k] + 1) > 50) && ((xmax[k] - xmin[k] + 1) < 300) && ((ymax[k] - ymin[k] + 1) > 150) && ((ymax[k] - ymin[k] + 1) < 450))
			{
				int fusepoint = 0;
				for (int j = ymin[k]; j < ymax[k]; j++)
				{
					uchar* current = waterShed.ptr< uchar>(j);
					for (int i = xmin[k]; i < xmax[k]; i++)
					{
						if (current[i] == 255)
							fusepoint++;
					}
				}
				fuseratio[k] = float(fusepoint) / ((xmax[k] - xmin[k] + 1) * (ymax[k] - ymin[k] + 1));
			}
		}

		Size dsize = Size(108, 128);
		// ��������ֵ������λ�û���
		for (int i = 0; i < label; i++)
		{
			if ((fuseratio[i] < 0.58))
			{
				// �߶ȵ���
				Mat rROI = Mat(dsize, CV_8UC1);
				resize(waterShed(Rect(xmin[i], ymin[i], xmax[i] - xmin[i], ymax[i] - ymin[i])), rROI, dsize);
				imshow("��������", rROI);

				// ģ��ƥ��
				Mat result;
				for (int mp = 0; mp < 5; mp++)
				{
					matchTemplate(rROI, mu[mp], result, CV_TM_SQDIFF_NORMED);
					simi[mp] = result.ptr<float>(0)[0];
				}

				// Ѱ�����ƥ��
				flagx2 = flagx1;
				flagx1 = flagx;
				flagx = 0;
				flag = simi[0];
				for (int j = 1; j < 5; j++)
				{
					if (simi[j] < flag)
					{
						flagx = j;
						flag = simi[j];
					}
				}

				cv::Point end = cv::Point(xmin[i], ymin[i] - 15);   // �ӱ�ǩ��λ�á�
				str1 = "(" + to_string(xmin[i]) + "," + to_string(ymin[i]) + ") " + " (" + to_string(xmax[i]) + "," + to_string(ymax[i]) + ")";
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0), 2);
				putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.5, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(xmin[i] - 80, ymin[i] + 20);   // �ӱ�ǩ���ֲ���ǡ�
				str1 = "Hand" + to_string(flagx + 1);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 20);   // �ӱ�ǩ����ɫ������
				str1 = "Skin area ratio: " + to_string(fuseratio[i]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 40);   // �ӱ�ǩ������
				str1 = "Height: " + to_string(ymax[i] - ymin[i]) + "  Width: " + to_string(xmax[i] - xmin[i]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 60);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_1: " + to_string(simi[0]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 80);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_2: " + to_string(simi[1]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 100);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_3: " + to_string(simi[2]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);


				end = cv::Point(20, 120);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_4: " + to_string(simi[3]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				end = cv::Point(20, 140);   // �ӱ�ǩ������ ƥ��ȡ�
				str1 = "Suitability_5: " + to_string(simi[4]);
				putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				//putText(frame, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);

				if (flagx == 0)
				{
					curpointx = xmin[i];
					curpointy = ymin[i];

					int dx = curpointx - prepointx;
					int dy = curpointy - prepointy;

					prepointx = curpointx;
					prepointy = curpointy;
					mouse_event(MOUSEEVENTF_MOVE, k * dx, k * dy, 0, 0);	// ����ƶ�

					end = cv::Point(20, 400);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_MOVE";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}

				if (flagx == 1 && flagx1 != 1 && flagx2 != 1)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_CLICK";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 2 && flagx1 != 2 && flagx2 != 2)
				{
					mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);			// ����Ҽ�����
					mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);			// ����Ҽ��ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_RIGHT_CLICK";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 3)
				{
					mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);			// ����������
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_DOWN";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}
				if (flagx == 4)
				{
					mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);			// �������ɿ�
					end = cv::Point(20, 440);   // �ӱ�ǩ������ ƥ��ȡ�
					str1 = "MOUSEEVENTF_LEFT_UP";
					putText(waterShed, str1, end, cv::FONT_HERSHEY_DUPLEX, 0.7, cv::Scalar(255, 0, 0), 2);
				}

				rectangle(waterShed, Point(xmin[i], ymin[i]), Point(xmax[i], ymax[i]), Scalar::all(255), 3, 8, 0);	// �ӿ�
				rectangle(frame, Point(xmin[i], ymin[i]), Point(xmax[i], ymax[i]), cv::Scalar(0, 255, 0), 3, 8, 0);	// �ӿ�
			}
		}
		imshow("��Ϣ��ʾ", waterShed);
		imshow("������ʾ", frame);


















		if (waitKey(1) >= 0)
			stop = true;
	}
	return 0;
}



