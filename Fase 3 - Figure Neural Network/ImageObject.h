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
	struct BoundingBox {
		Point top_left;
		Point top_right;
		Point bottom_left;
		Point bottom_right;
	};
private:
	BoundingBox bound_box;
	vector<Point> contour;
	vector<Point> boundingBox;
	vector<double> chainCode;
	Mat image;
public:
	vector<Point> getContour() { return contour; };
	vector<double> getChainCode() { return chainCode; };
	BoundingBox getBoundBox() { return bound_box; };
	Mat getImage() { return image; }
	
	ImageObject(Mat);
	ImageObject(Mat, Point2d);
	void calculateContour(Mat);	
	void calculateContour(Mat, Point2d);
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

};

