#include "search.h"
#include <iostream>

using namespace std;

CarPosition car;
Point park;
Point parkL;
Point parkR;

const char* carHeadWindow = "carHead";
const char* carTailWindow = "carTail";
const char* parkWindow = "park";

void searchCar(Mat& img, ColorSignature sigr, ColorSignature sigg) {
	Mat m_yuv;
	Mat carHead;
	Mat carTail;
	// ÏÈ×ª»»ÎªHSV¸ñÊ½
	cvtColor(img, m_yuv, cv::COLOR_BGR2HSV);
	// Í¨µÀ·ÖÀë
	Mat splits[3];
	split(m_yuv, splits);
	Mat imgh, imgs;

	inRange(splits[0], Scalar::all(sigg.m_uMin), Scalar::all(sigg.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigg.m_vMin), Scalar::all(sigg.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, carHead);
	imshow(carHeadWindow, carHead);

	inRange(splits[0], Scalar::all(sigr.m_uMin), Scalar::all(sigr.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigr.m_vMin), Scalar::all(sigr.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, carTail);
	imshow(carTailWindow, carTail);

	car.head = findCar(carHead);
	car.tail = findCar(carTail);
}

Point findCar(Mat& carImg) {
	Point Car;
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15), Point(0, 0));
	morphologyEx(carImg, carImg, MORPH_CLOSE, element);

	int nr = carImg.rows;
	int nl = carImg.cols;
	int point_num = 0, car_y = 0, car_x = 0;
	for (int yi = 0; yi < nr; yi++) {
		uchar* pimg = carImg.ptr<uchar>(yi);
		for (int xi = 0; xi < nl; xi++) {
			if (pimg[xi]) {
				car_y += yi;
				car_x += xi;
				point_num++;
			}
		}
	}
	if (point_num)
	{
		Car.x = car_x / point_num;
		Car.y = car_y / point_num;
	}
	else
		Car = Point(0, 0);

	return Car;
}

void searchPark(Mat & img, ColorSignature sigb) {
	Mat m_yuv;
	Mat parkArea;
	// ÏÈ×ª»»ÎªHSV¸ñÊ½
	cvtColor(img, m_yuv, cv::COLOR_BGR2HSV);
	// Í¨µÀ·ÖÀë
	Mat splits[3];
	split(m_yuv, splits);
	Mat imgh, imgs;

	inRange(splits[0], Scalar::all(sigb.m_uMin), Scalar::all(sigb.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigb.m_vMin), Scalar::all(sigb.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, parkArea);
	imshow(parkWindow, parkArea);

	park = findPark(parkArea);
}

Point findPark(Mat & parkImg) {
	Point Park;
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15), Point(0, 0));
	morphologyEx(parkImg, parkImg, MORPH_CLOSE, element);

	int nr = parkImg.rows;
	int nl = parkImg.cols;
	int point_num = 0, park_y = 0, park_x = 0;
	int pointL_num = 0, parkL_x = 0, parkL_y = 0;
	int pointR_num = 0, parkR_x = 0, parkR_y = 0;
	for (int yi = 0; yi < nr; yi++) {
		uchar* pimg = parkImg.ptr<uchar>(yi);
		for (int xi = 0; xi < nl; xi++) {
			if (pimg[xi]) {
				park_y += yi;
				park_x += xi;
				point_num++;
			}
		}
	}
	if (point_num) {
		Park.x = park_x / point_num;
		Park.y = park_y / point_num;
		for (int yi = 0; yi < nr; yi++) {
			uchar* pimg = parkImg.ptr<uchar>(yi);
			for (int xi = 0; xi < Park.x; xi++) {
				if (pimg[xi]) {
					parkL_y += yi;
					parkL_x += xi;
					pointL_num++;
				}
			}
		}
		parkL.x = parkL_x / pointL_num;
		parkL.y = parkL_y / pointL_num;

		for (int yi = 0; yi < nr; yi++) {
			uchar* pimg = parkImg.ptr<uchar>(yi);
			for (int xi = Park.x; xi < nl; xi++) {
				if (pimg[xi]) {
					parkR_y += yi;
					parkR_x += xi;
					pointR_num++;
				}
			}
		}
		parkR.x = parkR_x / pointR_num;
		parkR.y = parkR_y / pointR_num;
	}
	else {
		Park = Point(0, 0);
	}

	return Park;
}
