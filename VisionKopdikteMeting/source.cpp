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
	Mat image = imread("input/tomatenplant.jpg", CV_LOAD_IMAGE_COLOR);   
	if (!image.data)                           
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);

	Mat customDeltaXFiltered = TomatenAlgorithms::customDeltaX(gray_image, 30);
	Mat customDeltaYFiltered = TomatenAlgorithms::customDeltaY(gray_image, 30);
	Mat customDeltaY = TomatenAlgorithms::customDeltaY(gray_image, 0);
	Mat customDeltaX = TomatenAlgorithms::customDeltaX(gray_image, 0);
	Mat sobel = TomatenAlgorithms::sobel(gray_image);
	Mat canny;
	Canny(image, canny, 50, 200, 3);

	imshow("Source", image);
	imshow("Gray image", gray_image);
	imshow("Sobel", sobel);
	imshow("Canny", canny);
	imshow("customDeltaY", customDeltaY);
	imshow("customDeltaX", customDeltaX);
	imshow("customDeltaXFiltered", customDeltaXFiltered);
	imshow("customDeltaYFiltered", customDeltaYFiltered);
	
	TomatenAlgorithms::saveMatAsBMP("grayImage", gray_image);
	TomatenAlgorithms::saveMatAsBMP("customDeltaYFiltered", customDeltaYFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaXFiltered", customDeltaXFiltered);
	TomatenAlgorithms::saveMatAsBMP("customDeltaY", customDeltaY);
	TomatenAlgorithms::saveMatAsBMP("customDeltaX", customDeltaX);
	TomatenAlgorithms::saveMatAsBMP("sobel", sobel);
	TomatenAlgorithms::saveMatAsBMP("canny", canny);

	//dilate(customDeltaXFiltered, dilation_image, Mat(), Point(-1, 0), 3, 1, 10);
	//Imgproc.GaussianBlur(src, dst, new Size(45, 45), 0);	
	//GaussianBlur(customDeltaXFiltered, blur_image,Size(7, 7),0,0);

	//spational reasoning top to bottom
	Mat spationalReasoningTopToBottom = customDeltaXFiltered;
	TomatenAlgorithms::spatialReasoningTopToBottom(spationalReasoningTopToBottom);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningTopToBottom1", spationalReasoningTopToBottom);
	imshow("1 time spational reasoning top to bottom", spationalReasoningTopToBottom);
	

	//Run spational reasoning top to bottom a second time on the same image
	TomatenAlgorithms::spatialReasoningTopToBottom(spationalReasoningTopToBottom);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningTopToBottom2", spationalReasoningTopToBottom);
	imshow("2 times spational reasoning top to bottom", spationalReasoningTopToBottom);

	//spational reasoning bottom to top
	Mat spationalReasoningBottomToTop = customDeltaXFiltered;
	TomatenAlgorithms::spatialReasoningBottomToTop(spationalReasoningBottomToTop);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningBottomToTop1", spationalReasoningBottomToTop);
	imshow("1 time spational reasoning bottom to top", spationalReasoningBottomToTop);


	//Run spational reasoning bottom to top a second time on the same image
	TomatenAlgorithms::spatialReasoningTopToBottom(spationalReasoningBottomToTop);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningBottomToTop2", spationalReasoningBottomToTop);
	imshow("2 times spational reasoning bottom to top", spationalReasoningBottomToTop);

	//Run spational reasoning top to bottom on the bottom to top (second time) image
	TomatenAlgorithms::spatialReasoningTopToBottom(spationalReasoningTopToBottom);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningcombined", spationalReasoningTopToBottom);
	imshow("Spational reasoning combined", spationalReasoningTopToBottom);

	threshold(spationalReasoningTopToBottom, spationalReasoningTopToBottom, 0, 255, THRESH_BINARY);
	TomatenAlgorithms::saveMatAsBMP("spationalReasoningcombinedTreshold", spationalReasoningTopToBottom);
	imshow("Spational reasoning combined Treshold", spationalReasoningTopToBottom);

	//Mat test, stats, centroids;

	//connectedComponents(spationalReasoningTopToBottom, test);
	//connectedComponentsWithStats(spationalReasoningTopToBottom, test, stats, centroids);

	//TomatenAlgorithms::saveMatAsBMP("Blobtest", test);
	//imshow("BlobTest", test);


	waitKey(0);

	return 0;
}