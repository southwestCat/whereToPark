#include "search.h"
#include <iostream>

using namespace std;

const char* carHeadWindow = "carHead";
const char* carTailWindow = "rgb";

void search(Mat img, ColorSignature sigr, ColorSignature sigg, ColorSignature sigb) {
	cout << "search" << endl;
	cout << sigr.m_uMean << ends << sigr.m_vMean << endl;
}