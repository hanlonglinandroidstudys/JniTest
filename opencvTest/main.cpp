#include <iostream>
#include "opencv2\opencv.hpp"
#include "opencv\cv.h"
#include "opencv\highgui.h"
#include "opencv2\imgproc.hpp"

using namespace cv;
using namespace std;

#define NORMAL_WIDTH  360
#define NORMAL_HEIGHT 240



void handImage(Mat &image) {
	if (image.empty()) {
		std::cout << "ͼƬΪ�գ�" << std::endl;
		return;
	}
	imshow("src", image);
	Mat src_img = image;
	Mat dst;

	//��С��һ
	cv::Size RE_SIZE(NORMAL_WIDTH, NORMAL_HEIGHT);
	resize(src_img, dst, RE_SIZE);
	imshow("��С��һͼ", dst);

	//ͼ��ҶȻ�
	Mat gray_img;
	cvtColor(dst, gray_img, CV_BGR2GRAY);
	imshow("�Ҷ�ͼ", gray_img);

	//�Ҷ�ͼ��ֵ��
	/*
	 f(i,j)=   0 (gray<=100)
			   255(gray>100)
	 */
	Mat gray2_img;
	threshold(gray_img, gray2_img, 100, 255, CV_THRESH_BINARY);
	imshow("��ֵ��ͼ", gray2_img);

	//����
	Mat peng_img;
	Mat erodeElement = getStructuringElement(MORPH_RECT, cv::Size(10, 10));
	erode(gray2_img, peng_img, erodeElement);
	imshow("����ͼ", peng_img);

	//�������
	Mat dst_img; //���֤��
	vector<vector<cv::Point>> conturs;
	vector<Rect> rects;
	int max = 0;
	findContours(peng_img, conturs, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (int i = 0; i < conturs.size(); i++) {
		Rect rect = boundingRect(conturs.at(i));
		rectangle(peng_img, rect, Scalar(0, 0, 255));

		if (rect.width >= rect.height * 8) {
			rects.push_back(rect);
			rectangle(peng_img, rect, Scalar(0, 0, 255));
			dst_img = dst(rect);
		}

		/*dst_img = dst(rect);
		std::string title = std::to_string(i)+"_width_" + std::to_string(rect.width) + "_height_" + std::to_string(rect.height);
		imshow(title.c_str(), dst_img);*/
	}
	//ֻ��Ҫ������͵��Ǹ�����
	if (rects.size() == 1) {
		Rect rect = rects.at(0);
		dst_img = dst(rect);
	}
	else {
		int lowPoint = 0;
		Rect finalRect;
		for (int i = 0; i < rects.size();i++) {
			Rect rect = rects.at(i);
			Point point = rect.tl();
			if (point.y > lowPoint) {
				lowPoint = point.y;
				finalRect = rect;
			}
		}
		rectangle(dst, finalRect, Scalar(255, 255, 255));
		dst_img = dst(finalRect);
	}

	imshow("findCoutors", peng_img);
	if (!dst_img.empty())
		imshow("���֤���룺", dst_img);
}


int main() {

	std::cout << "start..." << std::endl;

	//Mat image = imread("hanlongin.jpg",1);
	Mat image = imread("../x64/Release/certno.png");
	handImage(image);
	waitKey(300000);
	return 0;
}
