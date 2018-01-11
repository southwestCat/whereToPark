#pragma once

#include "main.h"

typedef struct CarPosition {
	Point head = Point(0, 0);
	Point tail = Point(0, 0);
};

extern CarPosition car;
extern Point park;
extern Point parkL;
extern Point parkR;

void searchCar(Mat& img, ColorSignature sigr, ColorSignature sigg);
void searchPark(Mat & img, ColorSignature sigb);

Point findCar(Mat& carImg);
Point findPark(Mat& parkImg);
