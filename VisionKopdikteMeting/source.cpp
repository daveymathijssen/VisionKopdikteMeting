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
	Mat image = imread("input/Tomatenplant.jpg", CV_LOAD_IMAGE_COLOR);   
	if (!image.data)                           
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat gray_image, blur_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	Mat customDeltaY, customDeltaX, sobel, customDeltaXFiltered, customDeltaYFiltered, canny;
	customDeltaXFiltered = TomatenAlgorithms::customDeltaX(gray_image, 30);
	customDeltaYFiltered = TomatenAlgorithms::customDeltaY(gray_image, 30);
	customDeltaY = TomatenAlgorithms::customDeltaY(gray_image, 0);
	customDeltaX = TomatenAlgorithms::customDeltaX(gray_image, 0);
	sobel = TomatenAlgorithms::sobel(gray_image);
	Canny(image, canny, 50, 200, 3);

	imshow("Source", image);
	imshow("Gray image", gray_image);
	imshow("customDeltaXFiltered", customDeltaXFiltered);
	imshow("customDeltaYFiltered", customDeltaYFiltered);
	imshow("customDeltaY", customDeltaY);
	imshow("customDeltaX", customDeltaX);
	imshow("Sobel", sobel);
	imshow("Canny", canny);

	TomatenAlgorithms::saveMatAsBMP("grayImage", gray_image);
	TomatenAlgorithms::saveMatAsBMP("customDeltaYFiltered", customDeltaYFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaXFiltered", customDeltaXFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaY", customDeltaY);
	TomatenAlgorithms::saveMatAsBMP("customDeltaX", customDeltaX);
	TomatenAlgorithms::saveMatAsBMP("sobel", sobel);
	TomatenAlgorithms::saveMatAsBMP("canny", canny);

	//dilate(customDeltaXFiltered, dilation_image, Mat(), Point(-1, 0), 3, 1, 10);
	//Imgproc.GaussianBlur(src, dst, new Size(45, 45), 0);
	//GaussianBlur(src, dst, Size(5, 5), 0, 0);
	//Check is as follows: S=START zero pixel
	//0|11|7|12|0
	//0|5 |1|6 |0
	//0|0 |S|0 |0
	//0|4 |2|3 |0
	//0|10|8|9 |0
	int checkX[] = { 0,0,1,-1,-1,1,0,0,1,-1,-1,1};
	int checkY[] = { -1,1,1,1,-1,-1,-2,2,2,2,-2,-2};
	int checkAmount = 12;
	int found = 0;
	blur_image = customDeltaXFiltered;
	//5GaussianBlur(customDeltaXFiltered, blur_image,Size(7, 7),0,0);
	TomatenAlgorithms::testCircleAlgoritm(blur_image);

	imshow("Blurred_image", blur_image);
	waitKey(0);

	return 0;
}