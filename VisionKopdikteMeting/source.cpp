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
#include "Algorithms.h"

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
	Mat image = imread("PlantSchool.jpg", CV_LOAD_IMAGE_COLOR);   
	if (!image.data)                           
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	Mat customDeltaY, customDeltaX, sobel, customDeltaXFiltered, customDeltaYFiltered;
	customDeltaXFiltered = TomatenAlgorithms::customDeltaX(gray_image, 30);
	customDeltaYFiltered = TomatenAlgorithms::customDeltaY(gray_image, 30);
	customDeltaY = TomatenAlgorithms::customDeltaY(gray_image, 0);
	customDeltaX = TomatenAlgorithms::customDeltaX(gray_image, 0);
	sobel = TomatenAlgorithms::sobel(gray_image);

	imshow("Source", image);
	imshow("Gray image", gray_image);
	imshow("customDeltaXFiltered", customDeltaXFiltered);
	imshow("customDeltaYFiltered", customDeltaYFiltered);
	imshow("customDeltaY", customDeltaY);
	imshow("customDeltaX", customDeltaX);
	imshow("Sobel", sobel);

	TomatenAlgorithms::saveMatAsBMP("grayImage.bmp", gray_image);
	TomatenAlgorithms::saveMatAsBMP("customDeltaYFiltered.bmp", customDeltaYFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaXFiltered.bmp", customDeltaXFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaY.bmp", customDeltaY);
	TomatenAlgorithms::saveMatAsBMP("customDeltaX.bmp", customDeltaX);
	TomatenAlgorithms::saveMatAsBMP("sobel.bmp", sobel);

	waitKey(0);

	return 0;
}