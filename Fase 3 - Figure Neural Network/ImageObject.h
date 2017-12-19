#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "stdafx.h"
#include "avansvisionlib20.h"

using namespace std;
using namespace cv;

class ImageObject {

private:
	vector<Point> contour;
	vector<double> chainCode;
	Mat image;
	Point2f boundingBox[4];
	Size size;
	double surface, outline;
public:
	Point2f* getBoundingBox() { return boundingBox; };
	Size getSize() { return size; };
	vector<Point> getContour() { return contour; };
	vector<double> getChainCode() { return chainCode; };
	Mat getImage() { return image; }
	
	ImageObject(Mat,int);
	void calculateContourImage(Mat);
	void calculateContourObject(Mat);
	void calculateGeometry(Mat&);
	void findRotatedBoundingBox();
	
	/**
	@desc	Find a contour in binary_image with a given first pixel
	@param	-binary_image	Source image of the contour
			-firstPixel		Given first pixel of an object
	*/
	void findContour(Mat, Point2d);

	/**
	@desc	Find a contour in binary_image with a given first pixel
	@param	-binary_image	Source image of the contour
			-firstPixel		Given first pixel of an object
	*/
	void findBoundingBox();
	Mat rotate(Mat, double);
};

