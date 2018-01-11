#include "main.h"
#include "search.h"

const char* windows1 = "rgb";
const char* windows2 = "dst";
const char* windowsp = "parameter";
const char* controlWindow = "control";
const char* windowsR = "R";
const char* windowsG = "G";
const char* windowsB = "B";

Mat rgb;             // 原始图像     
Rect rect;           // 选中的区域
bool have = false;	//是否进行了学习
bool haveR = false;   // 是否进行了学习
bool haveG = false;
bool haveB = false;
float m_ratio = 0.8; // 选中区域的hs通道大于百分之多少算好
int r_switch = 0;
int g_switch = 0;
int b_switch = 0;
int start_switch = 0;
int isSearch = 0;

ColorSignature sigr, sigg, sigb;

void r_switch_off_function() {
	cout << "r_switch_off_function" << endl;
}

void r_switch_on_function() {
	cout << "r_awitch_on_function" << endl;
}

void r_switch_callback(int position) {
	if (position == 0) {
		r_switch_off_function();
	}
	else {
		r_switch_on_function();
	}
}

void g_switch_off_function() {
	cout << "g_switch_off_function" << endl;
}

void g_switch_on_function() {
	cout << "g_switch_on_function" << endl;
}

void g_switch_callback(int position) {
	if (position == 0) {
		g_switch_off_function();
	}
	else {
		g_switch_on_function();
	}
}


void b_switch_off_function() {
	cout << "b_switch_off_function" << endl;
}

void b_switch_on_function() {
	cout << "b_switch_on_function" << endl;
}

void b_switch_callback(int position) {
	if (position == 0) {
		b_switch_off_function();
	}
	else {
		b_switch_on_function();
	}
}

void start_switch_off_function() {
	cout << "Imagine initial" << endl;
	isSearch = 0;
}

void start_switch_on_function() {
	isSearch = 1;
}

void start_switch_callback(int position) {
	if (position == 0) {
		start_switch_off_function();
	}
	else {
		start_switch_on_function();
	}
}

void calcRatios8(Mat& uu, Mat& vv, struct ColorSignature *sig, float ratios[])
{
	int n = 0, counts[4];
	counts[0] = counts[1] = counts[2] = counts[3] = 0;

	for (int i = 0; i<uu.rows; i++)
	{
		for (int j = 0; j<uu.cols; j++)
		{
			uchar m_u = uu.ptr<uchar>(i)[j];
			uchar m_v = vv.ptr<uchar>(i)[j];
			if (m_u>sig->m_uMin)
				counts[0]++;

			if (m_u<sig->m_uMax)
				counts[1]++;

			if (m_v>sig->m_vMin)
				counts[2]++;

			if (m_v<sig->m_vMax)
				counts[3]++;

			n++;
		}

	}

	// calc ratios
	ratios[0] = (float)counts[0] / n;
	ratios[1] = (float)counts[1] / n;
	ratios[2] = (float)counts[2] / n;
	ratios[3] = (float)counts[3] / n;
	// calc mean (because it's cheap to do it here)
	sig->m_uMean = (sig->m_uMin + sig->m_uMax) / 2;
	sig->m_vMean = (sig->m_vMin + sig->m_vMax) / 2;
}

Mat HSVGetR(Mat& rgb)
{
	Mat m_hsv;
	// 先转换为HSV格式
	cvtColor(rgb, m_hsv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_hsv, splits);
	//// 对每个通道进行处理
	int scale = 0;
	float ratios[4];
	// 取得hs的上下限
	for (scale = 1 << 7, sigr.m_uMin = sigr.m_uMax = sigr.m_vMin = sigr.m_vMax = 0; scale != 0; scale >>= 1)
	{
		calcRatios8(splits[0], splits[1], &sigr, ratios);
		if (ratios[0]>m_ratio)
			sigr.m_uMin += scale;
		else
			sigr.m_uMin -= scale;

		if (ratios[1]>m_ratio)
			sigr.m_uMax -= scale;
		else
			sigr.m_uMax += scale;

		if (ratios[2]>m_ratio)
			sigr.m_vMin += scale;
		else
			sigr.m_vMin -= scale;

		if (ratios[3]>m_ratio)
			sigr.m_vMax -= scale;
		else
			sigr.m_vMax += scale;
	}

	int range = 2.5;
	sigr.m_uMin = sigr.m_uMean + (sigr.m_uMin - sigr.m_uMean)*range;
	sigr.m_uMax = sigr.m_uMean + (sigr.m_uMax - sigr.m_uMean)*range;
	range = 3.5;
	sigr.m_vMin = sigr.m_vMean + (sigr.m_vMin - sigr.m_vMean)*range;
	sigr.m_vMax = sigr.m_vMean + (sigr.m_vMax - sigr.m_vMean)*range;
	haveR = true;
	return Mat();
}

Mat HSVGetG(Mat& rgb)
{
	Mat m_hsv;
	// 先转换为HSV格式
	cvtColor(rgb, m_hsv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_hsv, splits);
	//// 对每个通道进行处理
	int scale = 0;
	float ratios[4];
	// 取得hs的上下限
	for (scale = 1 << 7, sigg.m_uMin = sigg.m_uMax = sigg.m_vMin = sigg.m_vMax = 0; scale != 0; scale >>= 1)
	{
		calcRatios8(splits[0], splits[1], &sigg, ratios);
		if (ratios[0]>m_ratio)
			sigg.m_uMin += scale;
		else
			sigg.m_uMin -= scale;

		if (ratios[1]>m_ratio)
			sigg.m_uMax -= scale;
		else
			sigg.m_uMax += scale;

		if (ratios[2]>m_ratio)
			sigg.m_vMin += scale;
		else
			sigg.m_vMin -= scale;

		if (ratios[3]>m_ratio)
			sigg.m_vMax -= scale;
		else
			sigg.m_vMax += scale;
	}

	int range = 2.5;
	sigg.m_uMin = sigg.m_uMean + (sigg.m_uMin - sigg.m_uMean)*range;
	sigg.m_uMax = sigg.m_uMean + (sigg.m_uMax - sigg.m_uMean)*range;
	range = 3.5;
	sigg.m_vMin = sigg.m_vMean + (sigg.m_vMin - sigg.m_vMean)*range;
	sigg.m_vMax = sigg.m_vMean + (sigg.m_vMax - sigg.m_vMean)*range;
	haveR = true;
	return Mat();
}

Mat HSVGetB(Mat& rgb)
{
	Mat m_hsv;
	// 先转换为HSV格式
	cvtColor(rgb, m_hsv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_hsv, splits);
	//// 对每个通道进行处理
	int scale = 0;
	float ratios[4];
	// 取得hs的上下限
	for (scale = 1 << 7, sigb.m_uMin = sigb.m_uMax = sigb.m_vMin = sigb.m_vMax = 0; scale != 0; scale >>= 1)
	{
		calcRatios8(splits[0], splits[1], &sigb, ratios);
		if (ratios[0]>m_ratio)
			sigb.m_uMin += scale;
		else
			sigb.m_uMin -= scale;

		if (ratios[1]>m_ratio)
			sigb.m_uMax -= scale;
		else
			sigb.m_uMax += scale;

		if (ratios[2]>m_ratio)
			sigb.m_vMin += scale;
		else
			sigb.m_vMin -= scale;

		if (ratios[3]>m_ratio)
			sigb.m_vMax -= scale;
		else
			sigb.m_vMax += scale;
	}

	int range = 2.5;
	sigb.m_uMin = sigb.m_uMean + (sigb.m_uMin - sigb.m_uMean)*range;
	sigb.m_uMax = sigb.m_uMean + (sigb.m_uMax - sigb.m_uMean)*range;
	range = 3.5;
	sigb.m_vMin = sigb.m_vMean + (sigb.m_vMin - sigb.m_vMean)*range;
	sigb.m_vMax = sigb.m_vMean + (sigb.m_vMax - sigb.m_vMean)*range;
	haveB = true;
	return Mat();
}

Mat HSVUpdateR(Mat& rgb)
{
	Mat m_yuv;
	Mat m_ret;
	// 先转换为HSV格式
	cvtColor(rgb, m_yuv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_yuv, splits);
	Mat imgh, imgs;
	inRange(splits[0], Scalar::all(sigr.m_uMin), Scalar::all(sigr.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigr.m_vMin), Scalar::all(sigr.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, m_ret);
	return m_ret;
}

Mat HSVUpdatwG(Mat& rgb) {
	Mat m_yuv;
	Mat m_ret;
	// 先转换为HSV格式
	cvtColor(rgb, m_yuv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_yuv, splits);
	Mat imgh, imgs;
	inRange(splits[0], Scalar::all(sigg.m_uMin), Scalar::all(sigg.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigg.m_vMin), Scalar::all(sigg.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, m_ret);
	return m_ret;
}

Mat HSVUpdateB(Mat& rgb) {
	Mat m_yuv;
	Mat m_ret;
	// 先转换为HSV格式
	cvtColor(rgb, m_yuv, cv::COLOR_BGR2HSV);
	// 通道分离
	Mat splits[3];
	split(m_yuv, splits);
	Mat imgh, imgs;
	inRange(splits[0], Scalar::all(sigb.m_uMin), Scalar::all(sigb.m_uMax), imgh);
	inRange(splits[1], Scalar::all(sigb.m_vMin), Scalar::all(sigb.m_vMax), imgs);
	cv::bitwise_and(imgh, imgs, m_ret);
	return m_ret;
}

void onMouse(int event, int x, int y, int flags, void *ustc)
{
	static Point pre_pt(-1, -1);
	static Point cur_pt(-1, -1);

	if (event == CV_EVENT_LBUTTONDOWN)
	{
		pre_pt = Point(x, y);
	}
	else if (event == CV_EVENT_MOUSEMOVE && (flags & CV_EVENT_FLAG_LBUTTON))
	{
		cur_pt = Point(x, y);
		// rectangle(rgb, pre_pt, cur_pt, Scalar::all(255));
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		rect = Rect(pre_pt, cur_pt);
		Mat imgroi = rgb(rect);
		if (r_switch) {
			HSVGetR(imgroi);
			imshow("imgroi", imgroi);
		}
		if (g_switch) {
			HSVGetG(imgroi);
			imshow("imgroi", imgroi);
		}
		if (b_switch) {
			HSVGetB(imgroi);
			imshow("imgroi", imgroi);
		}
	}
}

int main()
{
#if PHOTO_VIDEO
	rgb = imread("D:\\1.jpg");
	if (rgb.data == NULL)
	{
		cout << "read failed" << endl;
		return 0;
	}

	namedWindow(windows1);
	namedWindow(windows2);
	setMouseCallback(windows1, onMouse);

	imshow(windows1, rgb);

	waitKey(0);
	return 1;
#else 
	bool first = true;
	VideoCapture capture(1);

	if (!capture.isOpened())
	{
		cout << "can't open camera!" << endl;
		waitKey(0);
		return 0;
	}

	namedWindow(windows1);
	namedWindow(windows2);
	setMouseCallback(windows1, onMouse);
	namedWindow(controlWindow, CV_WINDOW_AUTOSIZE);
	cvCreateTrackbar("switch_r", controlWindow, &r_switch, 1, r_switch_callback);
	cvCreateTrackbar("switch_g", controlWindow, &g_switch, 1, g_switch_callback);
	cvCreateTrackbar("switch_b", controlWindow, &b_switch, 1, b_switch_callback);
	cvCreateTrackbar("start", controlWindow, &start_switch, 1, start_switch_callback);

	while (1)
	{
		capture >> rgb;

		if (rgb.data == NULL)
			continue;

		setMouseCallback(windows1, onMouse);

		if (isSearch == 0) {
			if (r_switch) {
				Mat outR = HSVUpdateR(rgb);
				imshow(windowsR, outR);
			}
			if (g_switch) {
				Mat outG = HSVUpdatwG(rgb);
				imshow(windowsG, outG);
			}
			if (b_switch) {
				Mat outB = HSVUpdateB(rgb);
				imshow(windowsB, outB);
			}

			imshow(windows1, rgb);
		}
		else {
			searchCar(rgb, sigr, sigg);
			searchPark(rgb, sigb);

			circle(rgb, car.head, 10, Scalar(255, 0, 0), -1, 8, 0);
			circle(rgb, car.tail, 10, Scalar(255, 0, 0), -1, 8, 0);
			circle(rgb, park, 10, Scalar(255, 0, 255), -1, 8, 0);
			circle(rgb, parkL, 10, Scalar(0, 215, 255), -1, 8, 0);
			circle(rgb, parkR, 10, Scalar(0, 215, 255), -1, 8, 0);
			imshow(windows1, rgb);
			cout << car.head << "    " << car.tail << endl;
		}

		int c = waitKey(100);
		if (c == 27)
			break;
	}

	return 1;
#endif
}
