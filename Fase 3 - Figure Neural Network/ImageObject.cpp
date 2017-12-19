#include "stdafx.h"
#include "ImageObject.h"
#include <cmath> 

#define _USE_MATH_DEFINES

ImageObject::ImageObject(Mat i, int type)
{
	image = i;

	switch (type)
	{
	case 0:
		calculateContourImage(i);
		break;
	case 1:
		calculateContourObject(i);
		findRotatedBoundingBox();
		calculateContourObject(i);
		break;
	}
}

void ImageObject::calculateGeometry(Mat& evaluation)
{
	Point2f A = boundingBox[0];
	Point2f B = boundingBox[1];
	Point2f D = boundingBox[3];

	double AB = sqrt(pow(abs(A.x - B.x), 2) + pow(abs(A.y - B.y), 2));
	double AD = sqrt(pow(abs(A.x - D.x), 2) + pow(abs(A.y - D.y), 2));
	Point min_loc, max_loc;
	double min, max;
	int Offset = 20;
	equalSize = true;

	minMaxLoc(evaluation, &min, &max, &min_loc, &max_loc);
	

	if (AB > AD)
	{
		size = Size(AB, AD);
	}
	else
	{
		size = Size(AD, AB);
	}
	cout << size << endl;




	switch (max_loc.y)
	{
	case 0:
		//Cirkel
		surface = (atan(1) * 4) * size.height * size.width;
		if ((surface - Offset) < (atan(1) * 4) * size.height * size.height || (surface + Offset) > (atan(1) * 4) * size.height * size.height)
		{
			equalSize = false;
		}
		break;
	case 1:
		//Vierkant
		surface = size.height * size.width;
		if ((surface - Offset) < pow(size.height, 2) || (surface + Offset) > pow(size.height, 2))
		{
			equalSize = false;
		}
		break;
	case 2:
		//Driehoek
		surface = (size.height * size.width) / 2;
		equalSize = false;
		break;
	}
}

void ImageObject::calculateContourImage (Mat binary_image)
{
	Mat labeled_image;
	// Get blob info
	vector<Point2d *> firstPixels;
	vector<Point2d *> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary_image, labeled_image, firstPixels, posVec, areaVec);

	// Find contour
	for (Point2d* firstPixel : firstPixels)
	{
		findContour(binary_image, Point2d(firstPixel->x, firstPixel->y));
	}
}

void ImageObject::calculateContourObject(Mat binary_image)
{
	// Get blob info
	Mat labeled_image;
	vector<Point2d *> firstPixels;
	vector<Point2d *> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary_image, labeled_image, firstPixels, posVec, areaVec);

	int max = distance(areaVec.begin(), max_element(areaVec.begin(), areaVec.end()));
	findContour(binary_image, Point2d(firstPixels.at(max)->x, firstPixels.at(max)->y));
}

void ImageObject::findContour(Mat binary_image, Point2d firstPixel)
{
	int row = firstPixel.x;
	int colmn = firstPixel.y;
	int dir = 3;
	int arr_i = -1;
	vector<double> chain_code;
	vector<Point> sub_contour;

	int offset[8][2] = {
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
		{ 0, 1 },
		{ 1, 1 },
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
	};

	while (true) {
		for (int index = 0; index < 8; index++) {
			if (index == 0) {
				arr_i = dir - 2;
				if (arr_i == -1) {
					arr_i = 7;
				}
				if (arr_i == -2) {
					arr_i = 6;
				};
			}
			if (arr_i == 8) { arr_i = 0; }
			if (binary_image.at<__int16>(row + offset[arr_i][0], colmn + offset[arr_i][1]) > 0) {
				if (arr_i == 0) { chain_code.push_back(3); }
				if (arr_i == 1) { chain_code.push_back(2); }
				if (arr_i == 2) { chain_code.push_back(1); }
				if (arr_i == 3) { chain_code.push_back(0); }
				if (arr_i == 4) { chain_code.push_back(7); }
				if (arr_i == 5) { chain_code.push_back(6); }
				if (arr_i == 6) { chain_code.push_back(5); }
				if (arr_i == 7) { chain_code.push_back(4); }

				sub_contour.push_back(Point(colmn, row));
				row = row + offset[arr_i][0];
				colmn = colmn + offset[arr_i][1];
				dir = arr_i;
				break;
			}
			arr_i++;
		}
		if (colmn == firstPixel.y && row == firstPixel.x) {
			break;
		}
	}
	contour = sub_contour;
	chainCode = chain_code;
}

void ImageObject::findRotatedBoundingBox()
{
	// Find the rotated rectangles for each contour
	RotatedRect minRect(minAreaRect(contour));
	vector<vector<Point>> test;
	test.push_back(contour);

	// Draw contours + rotated rects
	Mat drawing = Mat::zeros(image.size(), CV_8UC3);
	Mat dst = Mat::zeros(image.size(), CV_8UC3);

	for (int i = 0; i < test.size(); i++)
	{
		Scalar color = Scalar(150, 150, 150);
		// contour
		drawContours(drawing, test, i, color, 1, 8, vector<Vec4i>(), 0, Point());

		// rotated rectangle
		minRect.points(boundingBox);
		for (int j = 0; j < 4; j++)
			line(drawing, boundingBox[j], boundingBox[(j + 1) % 4], color, 1, 8);

		//imshow("Before Rotation", drawing);
		//waitKey(1);

		dst = rotate(drawing, minRect.angle);
		image = dst;
	}

	//imshow("After Rotation", dst);
	//waitKey(1);
}

Mat ImageObject::rotate(Mat src, double angle)
{
	Mat dst;
	Point2f pt(src.cols / 2., src.rows / 2.);
	Mat r = getRotationMatrix2D(pt, angle, 1.0);
	warpAffine(src, dst, r, Size(src.cols, src.rows));
	return dst;
}