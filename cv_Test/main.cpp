#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\ml\ml.hpp>
#include <iostream>
#include <fstream>
#include <io.h>
using namespace cv;
using namespace std;

void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,
			   cv::Scalar& color, int thickness = 1, int lineType = 8);

int main()
{

	Mat M(400, 400, CV_8UC3, Scalar(0, 0, 0));       
	Point pStart(380, 100), pEnd(100, 250);       
	Scalar lineColor(0, 255, 255);    
	drawArrow(M, pStart, pEnd, 10, 45, lineColor);    
	
	imshow("draw arrow", M);    
	waitKey();     
}

void drawArrow(cv::Mat& img, cv::Point pStart, cv::Point pEnd, int len, int alpha,             
			   cv::Scalar& color, int thickness, int lineType)
{    
	const double PI = 3.1415926;    
	Point arrow;    
	//���� �� �ǣ���򵥵�һ�����������ͼʾ���Ѿ�չʾ���ؼ����� atan2 ��������������棩   
	double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));  
	line(img, pStart, pEnd, color, thickness, lineType);   
	//������Ǳߵ���һ�˵Ķ˵�λ�ã�����Ļ��������Ҫ����ͷ��ָ��Ҳ����pStart��pEnd��λ�ã� 
	arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);  
	line(img, pEnd, arrow, color, thickness, lineType);   
	arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);    
	line(img, pEnd, arrow, color, thickness, lineType);
}