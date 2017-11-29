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
/**
@desc	Find all bounding boxes from contours
@param	-contours		Output of found contours
@return a status code
*/
int allBoundingBoxes(vector<ImageObject>&);
int enclosedPixels(const vector	<Point>&, vector<Point>&);


// Globals
RNG rng(123456);
Mat image;
vector<Mat> ROIs;

int main(int argc, char** argv)
{
	//Load original image

	image = imread("./../Images/basisfiguren.jpg", CV_LOAD_IMAGE_COLOR);
	imshow("Original", image);
	//waitKey(0);

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
	///imshow("Filtered", image_filtered*255);

	Mat binary16S_image;
	binary_image.convertTo(binary16S_image, CV_16S);

	// Search for contours
	vector<ImageObject>* objects = new vector<ImageObject>();
	allContours(binary16S_image, *objects);
	allBoundingBoxes(*objects);

	/// Show Bounding Boxes
	///Mat bb_image(image.cols, image.rows, CV_8UC3, Scalar(0, 0, 0));
	///for (ImageObject object : *objects)
	///{
	///	object.findBoundingBox();

	///	Scalar color = Scalar(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255));
	///	//Draw Contour
	///	polylines(bb_image, object.getContour(), true, color, 1, 8);
	///	//Draw Bounding Box
	///	rectangle(bb_image, object.getBoundBox().top_left, object.getBoundBox().bottom_right, color, 1, 8, 0);
	///}
	///imshow("Bounding Boxes", bb_image);

	for (ImageObject object : *objects){
		object.getContour();
		int total_pixels = enclosedPixels(object.getContour(), object.getenclosedPixels());
	}
	//vector<Point>& regionPixels;
	//const vector<Point>& contourVec 
	
	//int total_pixels = enclosedPixels(contourVec, regionPixels);

	imshow("ROI 1", ROIs.at(1));
	
	//show16SImageStretch(binary16S_image,"16s");
	


	waitKey(0);

    return 0;
}

int allBoundingBoxes(vector<ImageObject>& objects)
{
	int nr = 1;
	for (ImageObject object : objects)
	{
		//Find bounding box
		object.findBoundingBox();

		Mat roi_image = image(Rect(object.getBoundBox().top_left,object.getBoundBox().bottom_right));
		ROIs.push_back(roi_image);
		string name = "./../Images/ROI/figure_";
		name += nr;
		name += ".jpg";
		imwrite(name, roi_image);
		nr++;
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

int enclosedPixels(const vector	<Point>& contourVec, vector<Point>& regionPixels) {
	cout << contourVec;





	return -1;
}

