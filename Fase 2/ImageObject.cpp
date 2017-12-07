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

	//bound_box->top_left = Point(minX, minY);
	//bound_box->top_right = Point(maxX, minY);
	//bound_box->bottom_left = Point(minX, maxY);
	//bound_box->bottom_right = Point(maxX, maxY);

	bound_box.top_left = Point(minX, minY);
	bound_box.top_right = Point(maxX, minY);
	bound_box.bottom_left = Point(minX, maxY);
	bound_box.bottom_right = Point(maxX, maxY);
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

void ImageObject::findInnerFirstPixel() {
	// check if the direction 0 or 7 is possible to start as first inner pixel
	Point fp = { -1, -1 };
	if (chainCode.at(0) == 0 && chainCode.at(1) == 0 || 7) {
		fp = contour.at(0);
		fp.x++;
		fp.y++;
	}
	else {
		cout << "Incorrect contour" << endl;
	}
	innerFirstPixel = fp;
}

vector<Point> ImageObject::getBoundaryFillContour() {
	Point temp;
	vector <Point> solidContour;
	for (int index = 0; index < contour.size(); index++) {
		solidContour.push_back(contour.at(index));
		if (chainCode.at(index) == 1 || 3 || 5 || 7) {
			if (chainCode.at(index) == 1) {
				// voeg toe boven
				temp = contour.at(index);
				temp.y--;
				solidContour.push_back(temp);
			}
			if (chainCode.at(index) == 3) {
				// voeg toe links
				temp = contour.at(index);
				temp.x--;
				solidContour.push_back(temp);
			}
			if (chainCode.at(index) == 5) {
				// voeg toe onder
				temp = contour.at(index);
				temp.y++;
				solidContour.push_back(temp);
			}
			if (chainCode.at(index) == 7) {
				// voeg toe rechts
				temp = contour.at(index);
				temp.x++;
				solidContour.push_back(temp);
			}
		}
	}
	return solidContour;
}

void ImageObject::findFloodFillPixels() {
	// [colm][row]
	Point pixels[8] = {
		{ -1, -1 },
		{ 0, -1 },
		{ 1, -1 },
		{ -1, 0 },
		{ 1, 0 },
		{ -1, 1 },
		{ 0, 1 },
		{ 1, 1 },
	};

	// Find the first inner pixel
	findInnerFirstPixel();

	vector<Point> regionPixels(getBoundaryFillContour());
	vector<vector<Point>> fillArray;
	Point tempPixel;

	// Set first pixel		at value 0
	fillArray.push_back(vector<Point> { innerFirstPixel });
	innerPixels.push_back(innerFirstPixel);

	// Cnumber = number of blocks labeled
	// Tnumber = number of blocks per label
	for (int Cnumber = 0; Cnumber < fillArray.size(); Cnumber++) {
		for (int Tnumber = 0; Tnumber < fillArray.at(Cnumber).size(); Tnumber++) {
			vector<Point> tempPixels;
			for (int index = 0; index < 8; index++) {
				// position a the new pixel
				tempPixel = fillArray.at(Cnumber).at(Tnumber) + pixels[index];

				// check if the new pixel is included in the region Pixel
				if (find(regionPixels.begin(), regionPixels.end(), tempPixel) == regionPixels.end()) {
					tempPixels.push_back(tempPixel);
					regionPixels.push_back(tempPixel);
					innerPixels.push_back(tempPixel);
				}
			}
			fillArray.push_back(tempPixels);
		}
	}
}

void ImageObject::getPixelRange(vector<Point> points,vector<int>& pixelRange) {
	//[min x][max x][min y][max y]
	// this method will return the min and max values of x and y
	pixelRange.push_back(points.at(0).x);
	pixelRange.push_back(points.at(0).x);
	pixelRange.push_back(points.at(0).y);
	pixelRange.push_back(points.at(0).y);

	for (int index = 0; index < points.size(); index++) {
		if (points.at(index).x < pixelRange.at(0)) {
			pixelRange[0] = points.at(index).x;
		}
		if (points.at(index).x > pixelRange.at(1)) {
			pixelRange[1] = points.at(index).x;
		}
		if (points.at(index).y < pixelRange.at(2)) {
			pixelRange[2] = points.at(index).y;
		}
		if (points.at(index).y > pixelRange.at(3)) {
			pixelRange[3] = points.at(index).y;
		}
	}
}

void ImageObject::printChainCode( vector<int> chain_code) {
	cout << "Chain code:" << endl;
	for (int code : chain_code) {
		cout << code;
	}
	cout << endl;
}

void ImageObject::printPixels(vector<Point> points) {
	// and method who is able to print a vector of points in the command
	vector<int> pixelRange;
	getPixelRange(points, pixelRange);
	int xmin = pixelRange.at(0);
	int xmax = pixelRange.at(1) + 1;
	int ymin = pixelRange.at(2);
	int ymax = pixelRange.at(3) + 1;

	vector <Point> newPointArray;
	Point minValue{ xmin,ymin };

	for (int index = 0; index < points.size(); index++) {
		newPointArray.push_back(points.at(index) - minValue);
	}

	Point temp;
	for (int indexy = 0; indexy < (ymax - ymin); indexy++) {
		for (int indexx = 0; indexx < (xmax - xmin); indexx++) {

			temp = (Point(indexx, indexy));
			if (std::find(newPointArray.begin(), newPointArray.end(), temp) != newPointArray.end()) {
				cout << "x";
			}
			else {
				cout << " ";
			}
		}
		cout << endl;
	}
}
