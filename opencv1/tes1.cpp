#include "pch.h"
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
//
//using namespace std;
//using namespace cv;
//
//void mirrorY_test(Mat src, Mat &dst)
//{
//	int row = src.rows;
//	int col = src.cols;
//	dst = src.clone();
//	for (int i = 0; i < col; i++) {
//		src.col(col - 1 - i).copyTo(dst.col(i));
//	}
//}
//
//int main()
//{
//	cv::VideoCapture cap(0);
//	if (!cap.isOpened())
//	{
//		cout << "can't open video\n";
//		return -1;
//	}
//	Mat frame,binImage,frame_HSV,frame_YCrCb;
//	Mat Cr, Cb, H;
//	vector<Mat> channels, channels1;
//
//	float simi[5];	// 模板匹配
//	float flag;		// 标志
//	int flagx = 0, flagx1 = 0, flagx2 = 0;		// 中间变量
//	float k = 1.5;	// 鼠标灵敏因子
//	int curpointx = 0, curpointy = 0, prepointx = 367, prepointy = 80;	// 鼠标初始位置
//	int xg_num = 0;
//
//	bool stop = false;
//	while (!stop)
//	{
//		cap >> frame;
//		mirrorY_test(frame, frame);
//		//imshow("frame_mirror", frame);
//		
//		cvtColor(frame, binImage, CV_BGR2GRAY);
//
//		frame.copyTo(frame_YCrCb);
//		cvtColor(frame_YCrCb, frame_YCrCb, CV_BGR2YCrCb);
//		frame.copyTo(frame_HSV);
//		cvtColor(frame_HSV, frame_HSV, CV_BGR2HSV);
//
//		split(frame_YCrCb, channels);
//		split(frame_HSV,channels1);
//
//		Cr = channels.at(1);	// 分离出【色调Cr】
//		Cb = channels.at(2);	// 分离出【饱和度Cb】
//		H = channels1.at(0);	// 分离出【H】
//
//		/*imshow("Cr", Cr);
//		imshow("Cb", Cb);
//		imshow("H", H);*/
//
//		erode(binImage, binImage, Mat());
//		imshow("erode", binImage);
//		dilate(binImage, binImage, Mat());
//		imshow("dilate", binImage);
//
//
//
//
//		
//
//
//
//
//
//		
//
//
//
//
//
//
//
//
//
//
//
//
//
//		if (waitKey(1) >= 0)
//			stop = true;
//	}
//	/*Mat img = imread("form.jpg");
//	namedWindow("dsf");
//	imshow("da", img);
//	waitKey(5000);*/
//	return 0;
//
//	
//
//}
//
//using namespace cv;
//
//int main()
//{
//	VideoCapture capture;
//	capture.open(0);
//	if (!capture.isOpened())
//	{
//		printf("can not open video file   \n");
//		return -1;
//	}
//	Mat frame, dst;
//	Mat kernel;
//	//开操作处理
//	kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
//
//	namedWindow("input");
//	namedWindow("output");
//	std::vector<std::vector<Point>> contours;
//	std::vector<Vec4i> hireachy;
//	Rect rect;
//	Point2f center;
//	float radius = 20;
//	bool stop = false;
//	while (capture.read(frame)&&!stop)
//	{
//		//blur(frame, dst, Size(5,5));
//		inRange(frame, Scalar(213, 0, 0), Scalar(255, 213, 213), dst);
//
//		imshow("d", dst);
//		imshow("f", frame);
//		//开操作
//		morphologyEx(dst, dst, MORPH_OPEN, kernel);
//		//获取边界
//		findContours(dst, contours, hireachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point(0, 0));
//		//框选面积最大的边界
//		if (contours.size() > 0)
//		{
//			double maxArea = 0;
//			for (int i = 0; i < contours.size(); i++)
//			{
//				double area = contourArea(contours[static_cast<int>(i)]);
//				if (area > maxArea)
//				{
//					maxArea = area;
//					rect = boundingRect(contours[static_cast<int>(i)]);
//					minEnclosingCircle(contours[static_cast<int>(i)], center, radius);
//				}
//			}
//		}
//		//矩形框
//		//rectangle(frame,rect, Scalar(0,255,0),2);
//		//圆形框
//		circle(frame, Point(center.x, center.y), (int)radius, Scalar(0, 255, 0), 2);
//		/*imshow("input", frame);
//		imshow("output", dst);*/
//
//		if (waitKey(1) >= 0)
//			stop = true;
//	}
//
//	capture.release();
//	return 0;
//}


//#include <opencv2/core/utility.hpp>
//#include "opencv2/video/tracking.hpp"
//#include "opencv2/imgproc.hpp"
//#include "opencv2/videoio.hpp"
//#include "opencv2/highgui.hpp"
//#include <iostream>
//#include <ctype.h>
//using namespace cv;
//using namespace std;
//Mat image;
//bool backprojMode = false;
//bool selectObject = false;
//int trackObject = 0;
//bool showHist = true;
//Point origin;
//Rect selection;
//int vmin = 10, vmax = 256, smin = 30;
//// User draws box around object to track. This triggers CAMShift to start tracking
//static void onMouse(int event, int x, int y, int, void*)
//{
//	if (selectObject)
//	{
//		selection.x = MIN(x, origin.x);
//		selection.y = MIN(y, origin.y);
//		selection.width = std::abs(x - origin.x);
//		selection.height = std::abs(y - origin.y);
//		selection &= Rect(0, 0, image.cols, image.rows);
//	}
//	switch (event)
//	{
//	case EVENT_LBUTTONDOWN:
//		origin = Point(x, y);
//		selection = Rect(x, y, 0, 0);
//		selectObject = true;
//		break;
//	case EVENT_LBUTTONUP:
//		selectObject = false;
//		if (selection.width > 0 && selection.height > 0)
//			trackObject = -1;   // Set up CAMShift properties in main() loop
//		break;
//	}
//}
//string hot_keys =
//"\n\nHot keys: \n"
//"\tESC - quit the program\n"
//"\tc - stop the tracking\n"
//"\tb - switch to/from backprojection view\n"
//"\th - show/hide object histogram\n"
//"\tp - pause video\n"
//"To initialize tracking, select the object with mouse\n";
//static void help()
//{
//	cout << "\nThis is a demo that shows mean-shift based tracking\n"
//		"You select a color objects such as your face and it tracks it.\n"
//		"This reads from video camera (0 by default, or the camera number the user enters\n"
//		"Usage: \n"
//		"   ./camshiftdemo [camera number]\n";
//	cout << hot_keys;
//}
//const char* keys =
//{
//	"{help h | | show help message}{@camera_number| 0 | camera number}"
//};
//int main(int argc, const char** argv)
//{
//	VideoCapture cap;
//	Rect trackWindow;
//	int hsize = 16;
//	float hranges[] = { 0,180 };
//	const float* phranges = hranges;
//	CommandLineParser parser(argc, argv, keys);
//	if (parser.has("help"))
//	{
//		help();
//		return 0;
//	}
//	int camNum = parser.get<int>(0);
//	cap.open(camNum);
//	if (!cap.isOpened())
//	{
//		help();
//		cout << "***Could not initialize capturing...***\n";
//		cout << "Current parameter's value: \n";
//		parser.printMessage();
//		return -1;
//	}
//	cout << hot_keys;
//	namedWindow("Histogram", 0);
//	namedWindow("CamShift Demo", 0);
//	setMouseCallback("CamShift Demo", onMouse, 0);
//	createTrackbar("Vmin", "CamShift Demo", &vmin, 256, 0);
//	createTrackbar("Vmax", "CamShift Demo", &vmax, 256, 0);
//	createTrackbar("Smin", "CamShift Demo", &smin, 256, 0);
//	Mat frame, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;
//	bool paused = false;
//	for (;;)
//	{
//		if (!paused)
//		{
//			cap >> frame;
//			if (frame.empty())
//				break;
//		}
//		frame.copyTo(image);
//		if (!paused)
//		{
//			cvtColor(image, hsv, COLOR_BGR2HSV);
//			if (trackObject)
//			{
//				int _vmin = vmin, _vmax = vmax;
//				inRange(hsv, Scalar(0, smin, MIN(_vmin, _vmax)),
//					Scalar(180, 256, MAX(_vmin, _vmax)), mask);
//				int ch[] = { 0, 0 };
//				hue.create(hsv.size(), hsv.depth());
//				mixChannels(&hsv, 1, &hue, 1, ch, 1);
//				if (trackObject < 0)
//				{
//					// Object has been selected by user, set up CAMShift search properties once
//					Mat roi(hue, selection), maskroi(mask, selection);
//					calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
//					normalize(hist, hist, 0, 255, NORM_MINMAX);
//					trackWindow = selection;
//					trackObject = 1; // Don't set up again, unless user selects new ROI
//					histimg = Scalar::all(0);
//					int binW = histimg.cols / hsize;
//					Mat buf(1, hsize, CV_8UC3);
//					for (int i = 0; i < hsize; i++)
//						buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180. / hsize), 255, 255);
//					cvtColor(buf, buf, COLOR_HSV2BGR);
//					for (int i = 0; i < hsize; i++)
//					{
//						int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows / 255);
//						rectangle(histimg, Point(i*binW, histimg.rows),
//							Point((i + 1)*binW, histimg.rows - val),
//							Scalar(buf.at<Vec3b>(i)), -1, 8);
//					}
//				}
//				// Perform CAMShift
//				calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
//				backproj &= mask;
//				RotatedRect trackBox = CamShift(backproj, trackWindow,
//					TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));
//				if (trackWindow.area() <= 1)
//				{
//					int cols = backproj.cols, rows = backproj.rows, r = (MIN(cols, rows) + 5) / 6;
//					trackWindow = Rect(trackWindow.x - r, trackWindow.y - r,
//						trackWindow.x + r, trackWindow.y + r) &
//						Rect(0, 0, cols, rows);
//				}
//				if (backprojMode)
//					cvtColor(backproj, image, COLOR_GRAY2BGR);
//				ellipse(image, trackBox, Scalar(0, 0, 255), 3, LINE_AA);
//			}
//		}
//		else if (trackObject < 0)
//			paused = false;
//		if (selectObject && selection.width > 0 && selection.height > 0)
//		{
//			Mat roi(image, selection);
//			bitwise_not(roi, roi);
//		}
//		imshow("CamShift Demo", image);
//		imshow("Histogram", histimg);
//		char c = (char)waitKey(10);
//		if (c == 27)
//			break;
//		switch (c)
//		{
//		case 'b':
//			backprojMode = !backprojMode;
//			break;
//		case 'c':
//			trackObject = 0;
//			histimg = Scalar::all(0);
//			break;
//		case 'h':
//			showHist = !showHist;
//			if (!showHist)
//				destroyWindow("Histogram");
//			else
//				namedWindow("Histogram", 1);
//			break;
//		case 'p':
//			paused = !paused;
//			break;
//		default:
//			;
//		}
//	}
//	return 0;
//}
