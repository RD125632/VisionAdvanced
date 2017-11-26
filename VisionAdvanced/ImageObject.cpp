#include "stdafx.h"
#include "ImageObject.h"


void ImageObject::findContour(Mat binary_image, Point2d firstPixel)
{
	//Add first pixel
	contour.push_back(firstPixel);

	Point2d currentPixel = firstPixel;
	int dir = 3;
	int arr_i = -1;
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

	// Find the chaincode
	for (int index = 0; index < 8; index++) {
		if (index == 0) {
			arr_i = dir - 2;
			if (arr_i == -1) { arr_i = 7; }
			if (arr_i == -2) { arr_i = 6; }
		}
		if (arr_i == 8) { arr_i = 0; }

		double newX = currentPixel.x + offset[arr_i][0];
		double newY = currentPixel.y + offset[arr_i][1];

		if (binary_image.at<__int16>(currentPixel.y + offset[arr_i][1], currentPixel.x + offset[arr_i][0]) > 0) {
			if (arr_i == 0) { chainCode.push_back(3); }
			if (arr_i == 1) { chainCode.push_back(2); }
			if (arr_i == 2) { chainCode.push_back(1); }
			if (arr_i == 3) { chainCode.push_back(0); }
			if (arr_i == 4) { chainCode.push_back(7); }
			if (arr_i == 5) { chainCode.push_back(6); }
			if (arr_i == 6) { chainCode.push_back(5); }
			if (arr_i == 7) { chainCode.push_back(4); }

			currentPixel.x = newX;
			currentPixel.y = newY;
			dir = arr_i;
			break;
		}
		arr_i++;
	}
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
