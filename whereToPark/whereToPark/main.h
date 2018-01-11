#pragma once

#include <opencv2\opencv.hpp>
#include <iostream>

#define PHOTO_VIDEO 0	//0 « ”∆µ1 «Õº∆¨

using namespace std;
using namespace cv;

typedef struct ColorSignature
{
	//ColorSignature()
	//{
	//	m_uMin = m_uMax = m_uMean = m_vMin = m_vMax = m_vMean = m_type = 0;
	//}

	int m_uMin;
	int m_uMax;
	int m_uMean;
	int m_vMin;
	int m_vMax;
	int m_vMean;
	int m_rgb;
	int m_type;
};

