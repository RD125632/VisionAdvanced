#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "stdafx.h"

using namespace std;
using namespace cv;

class ImageObject {
private:
	vector<Point> contour;
	vector<Point> boundingBox;
	vector<int> chainCode;
	double bendingEnergy;
public:
	vector<Point> getContour() { return contour; };
	vector<int> getChainCode() { return chainCode; };
	double getBendingEnergy() { return bendingEnergy; };

	/**
	@desc	Find a contour in binary_image with a given first pixel
	@param	-binary_image	Source image of the contour
			-firstPixel		Given first pixel of an object
	*/
	void ImageObject::findContour(Mat, Point2d);

	/**
	@desc	Find a contour in binary_image with a given first pixel
	@param	-binary_image	Source image of the contour
	-firstPixel		Given first pixel of an object
	*/
	void ImageObject::findBoundingBox();

	/**
	@desc	Calculate bending energy from chaincode
	*/
	void ImageObject::findBendingEnergy();

	/**
	@desc	Print the chaincode
	@param	- Chaincode
	*/
	void ImageObject::to_string(vector<int>);
};