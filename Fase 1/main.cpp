#include "stdafx.h"
#include "avansvisionlib.h"
#include "ImageObject.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>


using namespace std;
using namespace cv;

/**
	@desc	Find all contours in binary_image
	@param	-binary_image	Source image of the contours
			-contours		Output of found contours
	@return a status code
*/
int allContours(Mat, vector<ImageObject>&);

int main(int argc, char** argv)
{
	//Load original image
	Mat image;
	image = imread("./../Images/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);
	imshow("Original", image);

	//Original image -> grayscaled image
	Mat gray_image;
	cvtColor(image, gray_image, CV_BGR2GRAY);
	///imshow("gray", gray_image);

	// Grayscaled image -> black/white scaled image
	Mat binary_image;
	threshold(gray_image, binary_image, 200, 1, CV_THRESH_BINARY_INV);
	///imshow("bin", binary_image * 255);

	// Image Filtering
	Mat image_filtered;
	Mat kernel = (Mat_<int>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::erode(binary_image, image_filtered, kernel);
	cv::dilate(image_filtered, binary_image, kernel);
	imshow("Filtered", image_filtered*255);

	Mat binary16S_image;
	binary_image.convertTo(binary16S_image, CV_16S);

	// Search for contours
	vector<ImageObject> objects;
	allContours(binary16S_image, objects);

	for (ImageObject &object : objects)
	{
		//object.findBendingEnergy();
		cout << "At: " << object.getContour().at(0) << " the bending Energy = " << object.getBendingEnergy() << endl;
	}

	waitKey(0);

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



