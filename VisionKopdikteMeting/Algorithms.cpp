#include "Algorithms.h"

//https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html
Mat TomatenAlgorithms::houghLineTransform(Mat source)
{

	Mat dst, cdst, gray_image;
	cvtColor(source, gray_image, CV_BGR2GRAY);
	Canny(source, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI / 180, 100, 0, 0);

	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000 * (-b));
		pt1.y = cvRound(y0 + 1000 * (a));
		pt2.x = cvRound(x0 - 1000 * (-b));
		pt2.y = cvRound(y0 - 1000 * (a));
		line(cdst, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
	}
#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 50, 50, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		Vec4i l = lines[i];
		line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
	}
#endif

	return cdst;
}


//https://docs.opencv.org/2.4.13.3/doc/tutorials/imgproc/imgtrans/sobel_derivatives/sobel_derivatives.html
Mat TomatenAlgorithms::sobel(Mat grayImage) {

	Mat grad;
	Mat grad_x = TomatenAlgorithms::sobelX(grayImage);
	Mat grad_y = TomatenAlgorithms::sobelY(grayImage);

	addWeighted(grad_x, 0.5, grad_y, 0.5, 0, grad);
	return grad;
}

Mat TomatenAlgorithms::sobelX(Mat grayImage)
{
	Mat grad_x, abs_grad_x;

	int scale = 10;
	int delta = 0;
	int ddepth = CV_16S;

	/// Gradient X
	//Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
	Sobel(grayImage, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_x, abs_grad_x);

	return abs_grad_x;
}

Mat TomatenAlgorithms::sobelY(Mat grayImage)
{
	Mat grad_y, abs_grad_y;

	int scale = 10;
	int delta = 0;
	int ddepth = CV_16S;

	/// Gradient Y
	//Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
	Sobel(grayImage, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);
	convertScaleAbs(grad_y, abs_grad_y);

	return abs_grad_y;
}

Mat TomatenAlgorithms::customDeltaX(Mat grayImage, int filter)
{
	Mat deltaImageX = grayImage.clone();

	int x, p;
	for (int j = 0; j< grayImage.cols; j++)
	{
		for (int i = 0; i < grayImage.rows; i++)
		{
			p = static_cast<int>(grayImage.at<uchar>(i, j));

			if (j != grayImage.cols - 1)
			{
				x = static_cast<int>(grayImage.at<uchar>(i, (j + 1)));
			}

			//filter noise
			if (filter > 0) {
				if (abs(p - x) < filter)
					p = 0;
				else
					p = abs(p - x);
			}
			else {
				p = abs(p - x);
			}
			
			deltaImageX.at<uchar>(i, j) = p;

		}
	}

	return deltaImageX;
}

Mat TomatenAlgorithms::customDeltaY(Mat grayImage, int filter = 0)
{
	Mat deltaImageY = grayImage.clone();

	int y, p;
	for (int j = 0; j< grayImage.cols; j++)
	{
		for (int i = 0; i < grayImage.rows; i++)
		{
			p = static_cast<int>(grayImage.at<uchar>(i, j));

			if (i != grayImage.rows - 1)
			{
				y = static_cast<int>(grayImage.at<uchar>((i + 1), j));
			}

			//filter noise
			if (filter > 0) {
				if (abs(p - y) < filter)
					p = 0;
				else
					p = abs(p - y);
			}
			else {
				p = abs(p - y);
			}

			deltaImageY.at<uchar>(i, j) = p;

		}
	}

	return deltaImageY;
}

Mat TomatenAlgorithms::testCircleAlgoritm(Mat delta_image)
{
	//Check is as follows: S=START zero pixel
	//0|11|7|12|0
	//0|5 |1|6 |0
	//0|0 |S|0 |0
	//0|4 |2|3 |0
	//0|10|8|9 |0
	int checkX[] = { 0,0,1,-1,-1,1,0,0,1,-1,-1,1 };
	int checkY[] = { -1,1,1,1,-1,-1,-2,2,2,2,-2,-2 };
	int checkAmount = 12;
	int found = 0;
	for (int x = 0; x<delta_image.rows; x++)
	{
		for (int y = 0; y<delta_image.cols; y++)
		{
			//when black pixel
			if (delta_image.at<uchar>(y, x) == 0) {
				for (int i = 0; i < checkAmount; i++)
				{
					if (delta_image.at<uchar>((y + checkY[i]), (x + checkX[i])) > 0)
					{
						found++;
					}
					if (i == 2 && found >1)
					{
						delta_image.at<uchar>(y, x) = 255;
					}
					if (i == 6 && found > 4)
					{
						delta_image.at<uchar>(y, x) = 255;
					}
					if (i == 8 && found > 5)
					{
						delta_image.at<uchar>(y, x) = 255;
					}
					if (i == 12 && found>8)
					{
						delta_image.at<uchar>(y, x) = 255;
					}
				}
				found = 0;
			}
			//when black pixel
			else if (delta_image.at<uchar>(y, x) > 0) {
				for (int i = 0; i<checkAmount; i++)
				{
					if (delta_image.at<uchar>((y + checkY[i]), (x + checkX[i])) == 0)
					{
						found++;
					}
					if (i == 6 && found> 6)
					{
						delta_image.at<uchar>(y, x) = 0;
					}
				}
				found = 0;
			}
		}
	}
	return delta_image;
}


//func fill in horizontal gaps between lines for a given image, scanning lines from top to bottom
Mat TomatenAlgorithms::spatialReasoningTopToBottom(Mat & delta_image)
{
	const int checkAmount = 12; //check this amount of pixels in a column
	bool foundLine = false;
	const int fillValue = 254;

	for (int col = 0; col<delta_image.cols; col++)
	{
		for (int row = 0; row<delta_image.rows; row++)
		{
			//when the pixel is black proceed, ignore all pixels that are not black
			if (delta_image.at<uchar>(row, col) == 0) {
				foundLine = false;
				
				if (row > 2)
				{
					//check if there are two filled in pixels on top of the current pixel
					if ((delta_image.at<uchar>((row - 1), (col)) > 0) && (delta_image.at<uchar>((row - 2), (col)) > 0))
					{
						foundLine = true;
					}
				}

				//if a line is found, check if the right and left pixel (of the current pixel) are black, continue
				if(foundLine && ( delta_image.at<uchar>(row, col+1) == 0 && delta_image.at<uchar>(row, col-1) == 0))
				{
					//check if there are pixels in the current column and the left and right column. If so, fill in the current pixel
					for(int j=0; j<(checkAmount);j++)
					{
						if (delta_image.at<uchar>((row + j), (col+1)) > 0)
						{
							if ((delta_image.at<uchar>((row  +j+1), (col +1)) > 0) && (delta_image.at<uchar>((row +2+j), (col +1 )) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
						if (delta_image.at<uchar>((row + j), (col)) > 0)
						{
							if ((delta_image.at<uchar>((row+j+ 1), (col)) > 0) && (delta_image.at<uchar>((row+j+2), (col)) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
						if (delta_image.at<uchar>((row + j), (col - 1)) > 0)
						{
							if ((delta_image.at<uchar>((row +1+j), (col -1)) > 0) && (delta_image.at<uchar>((row +j+2), (col-1)) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
					}	
				}

				//reset foundLine variable
				foundLine = false;

			}
		}
	}
	
	return delta_image;
}

//func fill in horizontal gaps between lines for a given image, scanning lines from top to bottom
Mat TomatenAlgorithms::spatialReasoningBottomToTop(Mat & delta_image)
{
	const int checkAmount = 12; //check this amount of pixels in a column
	bool foundLine = false;
	const int fillValue = 254;

	for (int col = delta_image.cols - 1; col >= 0; col--)
	{
		for (int row = delta_image.rows - 1; row >= 0; row--)
		{
			//when the pixel is black proceed, ignore all pixels that are not black
			if (delta_image.at<uchar>(row, col) == 0) {
				foundLine = false;

				//check if there are two filled in pixels on top of the current pixel
				if ((delta_image.at<uchar>((row + 1), (col)) > 0) && (delta_image.at<uchar>((row + 2), (col)) > 0))
				{
					foundLine = true;
				}

				//if a line is found, check if the right and left pixel (of the current pixel) are black, continue
				if (foundLine && (delta_image.at<uchar>(row, col + 1) == 0 && delta_image.at<uchar>(row, col - 1) == 0))
				{
					//check if there are pixels in the current column and the left and right column. If so, fill in the current pixel
					for (int j = 0; j<(checkAmount); j++)
					{
						if (delta_image.at<uchar>((row - j), (col + 1)) > 0)
						{
							if ((delta_image.at<uchar>((row - j - 1), (col + 1)) > 0) && (delta_image.at<uchar>((row - 2 - j), (col + 1)) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
						if (delta_image.at<uchar>((row - j), (col)) > 0)
						{
							if ((delta_image.at<uchar>((row - j - 1), (col)) > 0) && (delta_image.at<uchar>((row - j - 2), (col)) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
						if (delta_image.at<uchar>((row - j), (col - 1)) > 0)
						{
							if ((delta_image.at<uchar>((row - 1 - j), (col - 1)) > 0) && (delta_image.at<uchar>((row - j - 2), (col - 1)) > 0))
							{
								delta_image.at<uchar>(row, col) = fillValue;
								foundLine = false;
							}
						}
					}
				}

				//reset foundLine variable
				foundLine = false;

			}
		}
	}

	return delta_image;
}

//func filter stem
Mat TomatenAlgorithms::filterStem(Mat & spacialImage) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//create new mat for filtered stem
	Mat filteredTomatoplant(spacialImage.rows, spacialImage.cols, CV_8UC1, Scalar::all(0));
	int largestHeight = 0;
	int secondLargestHeight = 0;
	int largestContourIndex = 0;
	int secondLargestContourIndex = 0;
	//find all contours
	findContours(spacialImage, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++)
	{
		//if rectangle height is bigger than largestheight, set new largestheight
		if (boundingRect(contours[i]).height > largestHeight) {
			largestHeight = boundingRect(contours[i]).height;
			largestContourIndex = i;
		}
	}
	for (int i = 0; i < contours.size(); i++)
	{
		//if rectangle height is bigger than secondlargestheight and smaller than largestheight, set new secondlargestheight
		if (boundingRect(contours[i]).height<largestHeight && boundingRect(contours[i]).height>secondLargestHeight) {
			secondLargestHeight = boundingRect(contours[i]).height;
			secondLargestContourIndex = i;
		}
	}
	//use white color
	Scalar color(255, 255, 255);
	cout << "largest area = " << largestHeight << endl;
	cout << "second largeset area= " << secondLargestHeight << endl;
	//Draw the largest and the second largest contour
	drawContours(filteredTomatoplant, contours, largestContourIndex, color, CV_FILLED, 8, hierarchy);
	drawContours(filteredTomatoplant, contours, secondLargestContourIndex, color, CV_FILLED, 8, hierarchy);


	int beginStemX, beginStemY;
	for (int j = 0; j< filteredTomatoplant.cols; j++)
	{
		for (int i = 0; i < filteredTomatoplant.rows; i++)
		{
			//when first filled pixel is found set startposition.
			if(filteredTomatoplant.at<uchar>(i, j)>0)
			{
				beginStemX = j;
				beginStemY = i;
				for(int k=j+2;k<filteredTomatoplant.rows;k++)
				{
					//when second filled pixel is found calculate difference
					if (filteredTomatoplant.at<uchar>(i, k) > 0)
					{
						// if difference is bigger than 10 pixels 
						if ((k + 2) - beginStemX > 10)
						{
							cout << "x begin = " << beginStemX << " Y " << beginStemY << " x end = " << (k + 2) << " Y " << j << " difference = " << (k + 2) - beginStemX << endl;
						}
						break;
					}
				}
			}
		}
	}
	return filteredTomatoplant;
}

void TomatenAlgorithms::saveMatAsBMP(string name, Mat source)
{
	imwrite("output/" + name + ".bmp", source);
}
