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
void getChainCode(vector <Point>, vector <int>&);
Point getFirstPixel(vector <Point>, vector <int>);
void getSolidContour(vector <Point>contourVec, vector <int>, vector<Point>&);
void printPixelArea(vector <Point>, Point);
void fill(vector <Point> , Point , vector<Point>&);
void getPixelRange(vector<Point>, vector<int>&);
void printPicture(vector <Point>);

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
	int returnValue = -1;
	// Get chaincode
	vector<int> chain_code;
	getChainCode(contourVec, chain_code);
	
	// get first pixel
	Point fp = getFirstPixel(contourVec, chain_code);

	// get a solid contour
	vector<Point> solidContour;
	getSolidContour(contourVec, chain_code, solidContour);
	
	// fill in contour
	fill(solidContour, fp, regionPixels);
	returnValue = regionPixels.size();

	// show the filled pixture
	printPicture(regionPixels);

	return returnValue;
}



void getChainCode(vector <Point>contourVec, vector <int>& chain_code) {
	//generate Chaincode
	Point offset[8] = {
		{ 1, 0 },
		{ 1, -1 },
		{ 0, -1 },
		{ -1, -1 },
		{ -1, 0 },
		{ -1, 1 },
		{ 0, 1 },
		{ 1, 1 },
	};

	// check wich code match the pixel contour
	for (int index = 1; index < contourVec.size(); index++) {
		for (int index2 = 0; index2 < 8; index2++) {
			if ((contourVec.at(index) - contourVec.at(index - 1)) == offset[index2]) {
				chain_code.push_back(index2);
				break;
			}
			if (index2 == 7) {
				// this is only if the contour is not 4 connected
				cout << "wrong contour" << endl;
			}
		}
	}
	
	// get the chaincode for the last and first pixel
	for (int index2 = 0; index2 < 8; index2++) {
		if ((contourVec.at(0) - contourVec.at(contourVec.size()-1)) == offset[index2]) {
			chain_code.push_back(index2);
			break;
		}
		if (index2 == 7) {
			// this is only if the contour is not 4 connected
			cout << "wrong contour" << endl;
		}
	}
}

Point getFirstPixel(vector <Point> contourVec, vector <int> chain_code ) {
	// check if the direction 0 or 7 is possible to start as first inner pixel
	Point fp = { -1, -1 };
	if (chain_code.at(0) == 0 && chain_code.at(1) == 0 || 7) {
		fp = contourVec.at(0);
		fp.x++;
		fp.y++;
	}else{
		// sorry
		cout << "incorrect contour" << endl;
	}
	return fp;
}

void getSolidContour(vector <Point> contourVec, vector <int> chain_code, vector<Point>& solidContour) {
	Point temp;
	vector <Point> tempArray;
	for (int index = 0; index < contourVec.size(); index++) {
		solidContour.push_back(contourVec.at(index));
		if (chain_code.at(index) == 1 || 3 || 5 || 7) {
			if (chain_code.at(index) == 1) {
				// voeg toe boven
				temp = contourVec.at(index);
				temp.y--;
				tempArray.push_back(temp);
			}
			if (chain_code.at(index) == 3) {
				// voeg toe links
				temp = contourVec.at(index);
				temp.x--;
				tempArray.push_back(temp);
			}
			if (chain_code.at(index) == 5) {

				// voeg toe onder
				temp = contourVec.at(index);
				temp.y++;
				tempArray.push_back(temp);
			}
			if (chain_code.at(index) == 7) {
				// voeg toe rechts
				temp = contourVec.at(index);
				temp.x++;
				tempArray.push_back(temp);
			}
		}
	}
	for (int index = 0; index < tempArray.size(); index++) {
		solidContour.push_back(tempArray.at(index));
	}
	//printPicture(solidContour);
}

void printPixelArea(vector <Point> contourVec, Point x) {
	// [colm][row]
	Point pixels[9] = {
		{ -1, -1 },
		{ 0, -1 },
		{ 1, -1 },
		{ -1, 0 },
		{ 0, 0 },
		{ 1, 0 },
		{ -1, 1 },
		{ 0, 1 },
		{ 1, 1 },
	};


	Point temp;
	for (int index = 0; index < 9; index++) {
		temp.x = x.x + pixels[index].x;
		temp.y = x.y + pixels[index].y;


		if (std::find(contourVec.begin(), contourVec.end(), temp )!= contourVec.end()) {
			//cout << "ja" << endl;/* v contains x */
			cout << "1";
		}
		else {
			cout << "0";
			//cout << "nee" << endl;
			/* v does not contain x */
		}

		if (index == 2 || index == 5 || index == 8) {
			cout << endl;
		}
	}
	cout << endl;
}

void fill(vector <Point> solidContour, Point fp, vector<Point>& regionPixels) {
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
	
	// some local variables
	vector < vector<Point> > tempRegionPixels;
	vector <Point> emptyVector;
	Point empty = { -1,-1 };
	emptyVector.push_back(empty);
	Point temp = fp;
	int counter = 1;
	int maxLim = 200;

	// fill an vector with ampy vectors which contain points
	for (int index = 0; index < maxLim+1; index++) {
		tempRegionPixels.push_back(emptyVector);
	}

	// set the first pixel to start
	tempRegionPixels.at(0).at(0) = fp;

	// insert the contour in our current temperary vector. The ccontour get the value maxLim
	for (int index = 0; index < solidContour.size(); index++) {
		tempRegionPixels.at(maxLim).push_back(solidContour.at(index));
		regionPixels.push_back(solidContour.at(index));
	}

	// Cnumber = number of blocks labeled
	// Tnumber = number of blocks per label

	//  fill algorithm
	for (int Cnumber = 0; Cnumber < tempRegionPixels.size(); Cnumber++) {
		for (int Tnumber = 0; Tnumber < tempRegionPixels.at(Cnumber).size(); Tnumber++) {
			for (int index = 0; index < 8; index++) {
				// this means a new vector at value counter will be filled
				if (tempRegionPixels.at(Cnumber).at(Tnumber) == Point(-1, -1)) {
					// Filling the contour is done
					return;
				}
				// position a the new pixel
				temp = tempRegionPixels.at(Cnumber).at(Tnumber) + pixels[index];

				// check if the new pixel is included in the region Pixel
				if (std::find(regionPixels.begin(), regionPixels.end(), temp) == regionPixels.end() ) {
					if (tempRegionPixels.at(Cnumber).at(0) == Point{ -1, -1 }) {
						tempRegionPixels.at(Cnumber).at(0) = temp;
					}else{
						tempRegionPixels.at(Cnumber).push_back(temp);
					}
					regionPixels.push_back(temp);
				}
			}
		}
	}
}

void getPixelRange(vector <Point> solidContour, vector<int>& pixelRange) {
	//[min x][max x][min y][max y]

	// this method will return the min and max values of x and y
	pixelRange.push_back(solidContour.at(0).x);
	pixelRange.push_back(solidContour.at(0).x);
	pixelRange.push_back(solidContour.at(0).y);
	pixelRange.push_back(solidContour.at(0).y);

	for (int index = 0; index < solidContour.size(); index++) {
		if (solidContour.at(index).x < pixelRange.at(0)) {
			pixelRange[0] = solidContour.at(index).x;
		}
		if (solidContour.at(index).x > pixelRange.at(1)) {
			pixelRange[1] = solidContour.at(index).x;
		}
		if (solidContour.at(index).y < pixelRange.at(2)) {
			pixelRange[2] = solidContour.at(index).y;
		}
		if (solidContour.at(index).y > pixelRange.at(3)) {
			pixelRange[3] = solidContour.at(index).y;
		}
	}

	return;
}

void printPicture(vector <Point> points) {
	// and method who is able to print a vector of points in the command
	vector<int> pixelRange;
	getPixelRange(points, pixelRange);
	int xmin = pixelRange.at(0);
	int xmax = pixelRange.at(1)+1;
	int ymin = pixelRange.at(2);
	int ymax = pixelRange.at(3)+1;

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
			}else{
				cout << " ";
			}
		}
		cout << endl;
	}
}