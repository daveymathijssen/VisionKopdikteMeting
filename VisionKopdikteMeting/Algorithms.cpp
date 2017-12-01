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
					if (i == 12 && found>10)
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

void TomatenAlgorithms::saveMatAsBMP(string name, Mat source)
{
	imwrite("output/" + name + ".bmp", source);
}
