// Fase 3 - Figure Neural Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "avansvisionlib20.h"
#include "ImageObject.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;

int loadTrainingSet();

/**
@desc	Find all contours in binary_image
@param	-binary_image	Source image of the contours
-contours		Output of found contours
@return a status code
*/
int allContours(Mat, vector<ImageObject>&);
/**
@desc	Find all bounding boxes from contours
@param	-contours		Output of found contours
@return a status code
*/
int allBoundingBoxes(vector<ImageObject>&);


// Globals
RNG rng(123456);
Mat image;

int main(int argc, char** argv)
{
	loadTrainingSet();


	//Original image -> grayscaled image
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	///imshow("gray", gray_image);

	// Grayscaled image -> black/white scaled image
	Mat binary_image;
	threshold(gray_image, binary_image, 200, 1, CV_THRESH_BINARY_INV);
	///imshow("bin", binary_image * 255);

	Mat binary16S_image;
	binary_image.convertTo(binary16S_image, CV_16S);

	// Search for contours
	vector<ImageObject>* objects = new vector<ImageObject>();
	allContours(binary16S_image, *objects);
	allBoundingBoxes(*objects);

	//show16SImageStretch(binary16S_image,"16s");

	waitKey(0);
	return 0;
}

int loadTrainingSet()
{
	ostringstream oss;
	oss << "./../Images/Figuren" << path;

	//Load original image
	image = imread(oss.str(), CV_LOAD_IMAGE_COLOR);
	imshow("Original", image);

}

int allBoundingBoxes(vector<ImageObject>& objects)
{
	for (ImageObject object : objects)
	{
		//Find bounding box
		object.findBoundingBox();

		Mat roi_image = image(Rect(object.getBoundBox().top_left, object.getBoundBox().bottom_right));
		ROIs.push_back(roi_image);
	}
	return 0;
}

int allContours(Mat binary_image, vector<ImageObject>& objects)
{
	// Get blob info
	Mat labeled_image;
	vector<Point2d *> firstPixels;
	vector<Point2d *> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary_image, labeled_image, firstPixels, posVec, areaVec);

	for (Point2d* firstPixel : firstPixels)
	{
		// Find contour
		cout << "Contour of (" << firstPixel->x << "," << firstPixel->y << ")" << endl;

		ImageObject object;
		object.findContour(binary_image, Point2d(firstPixel->x, firstPixel->y));
		objects.push_back(object);
	}

	return 0;
}
