#pragma once
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <string>

using namespace cv;
using namespace std;

class TomatenAlgorithms
{
public:
	static Mat houghLineTransform(Mat);
	static Mat sobel(Mat);
	static Mat sobelX(Mat);
	static Mat sobelY(Mat);
	static Mat customDeltaX(Mat, int);
	static Mat customDeltaY(Mat, int);
	static Mat testCircleAlgoritm(Mat);
	static Mat spatialReasoningBottomToTop(Mat &);
	static Mat spatialReasoningTopToBottom(Mat &);
	static Mat filterStem(Mat &);
	static void saveMatAsBMP(string,Mat);

};