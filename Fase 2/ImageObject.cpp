#include "stdafx.h"
#include "ImageObject.h"


void ImageObject::findContour(Mat binary_image, Point2d firstPixel)
{
	int row = firstPixel.x;
	int colmn = firstPixel.y;
	int dir = 3;
	int arr_i = -1;
	vector<int> chain_code;
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
	int minX = 100000, minY = 100000, maxX = 0 , maxY = 0;

	for (Point p : contour)
	{
		if (p.x < minX)			{ minX = p.x; } 
		else if (p.x > maxX )	{ maxX = p.x; }
		if (p.y < minY)			{ minY = p.y; }
		else if (p.y > maxY)	{ maxY = p.y; }
	}

	bound_box = BoundingBox{
		{ minX, minY },
		{ maxX, minY },
		{ minX, maxY },
		{ maxX, maxY }
	};
}

void ImageObject::findBendingEnergy()
{
	bendingEnergy = 0;
	for (int i = 1; i < chainCode.size(); i++)
	{
		if (i + 1 < contour.size())
			bendingEnergy += abs(chainCode[i] - chainCode[i - 1]);
		else
			bendingEnergy += abs(chainCode[i] - chainCode[0]);
	}
}

void ImageObject::to_string(vector<int> chain_code) {
	cout << "Chain code:" << endl;
	for (int code : chain_code) {
		cout << code;
	}
	cout << endl;
}
