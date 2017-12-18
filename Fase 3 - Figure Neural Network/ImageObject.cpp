#include "stdafx.h"
#include "ImageObject.h"


ImageObject::ImageObject(Mat i)
{
	image = i;
	calculateContour(image);
}

void ImageObject::calculateContour (Mat binary_image)
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


ImageObject::ImageObject(Mat i, Point2d fp)
{
	image = i;
	calculateContour(image, fp);
}

void ImageObject::calculateContour(Mat binary_image, Point2d fp)
{
	findContour(binary_image, fp);
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

void ImageObject::findBoundingBox()
{
	int minX = 100000, minY = 100000, maxX = 0, maxY = 0;

	for (Point p : contour)
	{
		if (p.x < minX) { minX = p.x; }
		else if (p.x > maxX) { maxX = p.x; }
		if (p.y < minY) { minY = p.y; }
		else if (p.y > maxY) { maxY = p.y; }
	}

	bound_box.top_left = Point(minX, minY);
	bound_box.top_right = Point(maxX, minY);
	bound_box.bottom_left = Point(minX, maxY);
	bound_box.bottom_right = Point(maxX, maxY);
}
