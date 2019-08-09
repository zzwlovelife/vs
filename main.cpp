//线段检测只能针对二值图像
#include <opencv2/core/core.hpp>  
#include<opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>  
#include <math.h>
using namespace cv;

Mat GrayImage;
Mat BinaryImage;
Mat SrcImage;
Mat CannyImage;
Mat ColorImage;

void to_draw_contours()
{
	/*
	const int IMAGE_WIDTH = 400;
	const int IMAGE_HEIGHT = 400;

	const int IMAGE_ROWS = 400;
	const int IMAGE_COLS = 400;

	//创建图像
	//SrcImage.create(IMAGE_WIDTH, IMAGE_HEIGHT, CV_8UC3);
	SrcImage.create(IMAGE_ROWS, IMAGE_COLS, CV_8UC3);
	//填充成白色
	rectangle(SrcImage, Rect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT), CV_RGB(255, 255, 255), 1,8,0);

	//定义圆心和半径
	Point CircleCenter;
	int nRadius;

	//画圆
	CircleCenter = Point(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	nRadius = 80;
	circle(SrcImage, CircleCenter, nRadius, CV_RGB(255, 255, 0),1,8,0);

	CircleCenter = Point(IMAGE_WIDTH / 4, IMAGE_HEIGHT / 2);
	nRadius = 30;
	circle(SrcImage, CircleCenter, nRadius, CV_RGB(255, 255, 255), 1, 8, 0);

	//定义矩形的两个点
	Point LeftTop, RightBottom;

	//画矩形
	LeftTop = Point(IMAGE_WIDTH / 2 + 20, 20);
	RightBottom = Point(IMAGE_WIDTH - 20, IMAGE_HEIGHT - 20);
	rectangle(SrcImage, LeftTop, RightBottom, CV_RGB(0, 255, 255), 1, 8, 0);

	LeftTop = Point(IMAGE_WIDTH / 2 + 60, 40);
	RightBottom = Point(IMAGE_WIDTH - 60, IMAGE_HEIGHT - 40);
	rectangle(SrcImage, LeftTop, RightBottom, CV_RGB(255, 255, 255), 1, 8, 0);
	*/
	SrcImage = cv::imread("1739131.jpg");
	Mat gray, smallImg(cvRound(SrcImage.rows /8), cvRound(SrcImage.cols /8), CV_8UC1);//smallImg已缩放

	//cvtColor(SrcImage, gray, CV_BGR2GRAY);//图片颜色格式转化，CV_BGR2GRAY是从BGR到gray，彩色到灰色
	//resize(gray, smallImg, smallImg.size(), 0, 0, INTER_LINEAR);//将灰色图像适应大小到smallImg中
	equalizeHist(smallImg, smallImg);//加强对比度，提高检测准确率
	namedWindow("原图");
	imshow("原图", SrcImage);
}

void RGB_to_GRAY()
{
	GrayImage.create(SrcImage.size(), CV_8UC1);
	cvtColor(SrcImage, GrayImage,0,0);
}


void to_cannyImage()
{
	CannyImage.create(SrcImage.size(), CV_8UC1);
	Canny(GrayImage, CannyImage, 30, 90);
}

void line_detection()
{
	std::vector<Vec2f> lines;

	double fRho = 1;
	double fTheta = CV_PI / 180;
	int MaxLineNumber = 50;
	double MinLineLen = 50;
	double MinLineGap = 10;
	//检测线段
	HoughLines(CannyImage, lines, fRho, fTheta, 50, MaxLineNumber, MinLineLen, MinLineGap);
	//绘制线段
	ColorImage.create(SrcImage.size(), CV_8UC3);
	cvtColor(CannyImage, ColorImage, 0,0);
	int i;
	for (i = 0; i < lines.size(); i++)
	{
		fRho = lines[i][0];
		fTheta = lines[i][1];
		Point pt1, pt2;
		double a = cos(fTheta);
		double b = sin(fTheta);
		double x0 = a * fRho;
		double y0 = b * fRho;
		pt1.x = cvRound(x0 + 100 * (-b));
		pt1.y = cvRound(y0 + 100 * (a));
		pt2.x = cvRound(x0 - 100 * (-b));
		pt2.y = cvRound(y0 - 100 * (a));
		line(ColorImage, pt1, pt2, Scalar(0, 0, 255), 1, LINE_AA);
	}
	namedWindow("直接检测");
	imshow("直线检测", ColorImage);
}

void line_segment_detection()
{
	std::vector<Vec4i>lines;
	HoughLinesP(CannyImage, lines, 1, CV_PI / 180, 50, 10);


	ColorImage.create(SrcImage.size(), CV_8UC3);
	cvtColor(CannyImage, ColorImage, 0,0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(ColorImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186, 88, 255), 1, LINE_AA);
	}

}

void circle_detection()
{
	std::vector<Vec3f>circles;
	HoughCircles(CannyImage, circles, HOUGH_GRADIENT, 1.5, 10, 200, 100, 0, 0);

	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		/*circle(ColorImage, center, 3, Scalar(0, 255, 0), 3, 8, 0);*/
		circle(ColorImage, center, radius, Scalar(155, 50, 255), 3, 8, 0);
	}
}

void display_dstImage()
{
	namedWindow("线段检测与圆检测");
	imshow("线段检测与圆检测", ColorImage);
}

void main()
{
	to_draw_contours();
	RGB_to_GRAY();
	to_cannyImage();
	line_segment_detection();
	circle_detection();
	display_dstImage();
	waitKey(0);
}
