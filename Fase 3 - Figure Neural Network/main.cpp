// Fase 3 - Figure Neural Network.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ImageObject.h"
#include "BPN.h"
#include "UnitTest.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <string>


#define PI 3.14159265
using namespace std;
using namespace cv;

void initialize();
void evaluateObject(Mat);
void printEvaluation(Mat&);
bool validateInput(Mat&, Mat&);
void createOutput(int, int, Mat&);
vector<double> calculateAngle(int, vector<int>);
void createChainCode(vector<int>&);
double GetAngleABC(Point a, Point b);


// Globals
vector<vector<ImageObject>> trainingImages;
vector<string> figureSets = { "cirkel", "vierkant", "driehoek" };
Figure_BPN bpn;
UnitTest unit;
Mat V0, W0;
int thresholdV = 30;

int main(int argc, char** argv)
{
	vector<int> chainCode;
	calculateAngle(7, chainCode);
	Mat OT;
	createOutput(10, 10, OT);
	initialize();

	//vector<ImageObject> testImages;
	//unit.loadTestFigures(testImages);

	VideoCapture cap(1);

	if (!cap.isOpened()){ 
		cout << "error" << endl;
		return -1; 
	}




	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	Mat image_filtered;
	while (1)
	{
		cout << endl;
		cout << "Plaats een object en druk op ENTER!!!!!" << endl;
		cin.ignore();

		Mat captured_image;
		cap >> captured_image;
		if (validateInput(captured_image, image_filtered))
		{
			Mat binairy_image1;
			image_filtered.convertTo(binairy_image1, CV_16S);

			evaluateObject(binairy_image1);
		}
	}
	waitKey(0);
	return 0;
}

bool validateInput(Mat& cap, Mat& image_filtered)
{

	Mat grey_image;
	cvtColor(cap, grey_image, CV_BGR2GRAY);
	Mat binary_image;
	threshold(grey_image, binary_image, thresholdV, 1, CV_THRESH_BINARY_INV);
	// Image Filtering
	Mat kernel = (Mat_<int>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::erode(binary_image, image_filtered, kernel);
	cv::dilate(image_filtered, binary_image, kernel);
	imshow("Object Cam", image_filtered * 255);
	waitKey(1);
	cout << "is this a valid picture? y/n?" << endl;
	char type;
	cin >> type;
	if (type == 'y') {
		return true;
	}
	return false;
}


void initialize() {
	bpn.loadTrainingFigures(figureSets, trainingImages);
	bpn.train(trainingImages, V0 ,W0);
}

void evaluateObject(Mat binary_image)
{
	// Get blob info
	Mat labeled_image;
	vector<Point2d *> firstPixels;
	vector<Point2d *> posVec;
	vector<int> areaVec;
	labelBLOBsInfo(binary_image, labeled_image, firstPixels, posVec, areaVec);

	int max = distance(areaVec.begin(), max_element(areaVec.begin(), areaVec.end()));

	// Find contour
	Mat evaluation;
	ImageObject IO = ImageObject(binary_image, Point2d(firstPixels.at(max)->x, firstPixels.at(max)->y));
	bpn.evaluateImage(IO.getChainCode(), V0, W0, evaluation);
	printEvaluation(evaluation);
}


void printEvaluation(Mat& evaluation)
{
	cout << endl;
	cout << "Object Evaluation:" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Cirkel:   " << roundf(evaluation.at<double>(0, 0) * 100) << "%" << endl;
	cout << "Vierkant: " << roundf(evaluation.at<double>(1, 0) * 100) << "%" << endl;
	cout << "Driehoek: " << roundf(evaluation.at<double>(2, 0) * 100) << "%" << endl;
	cout << "----------------------------------------" << endl;
}

void createOutput(int amountOfSets, int inSet, Mat& OTset) {
	int totalTrainingInputImages = amountOfSets * inSet;
	OTset = (Mat_<double>(totalTrainingInputImages, inSet));
	for (int index = 0; index < totalTrainingInputImages; index++) {
		for (int index1 = 0; index1 < inSet; index1++) {
			if (index1 == index/10) {
				setEntry(OTset, index, index1, 1);
			}else{
				setEntry(OTset, index, index1, 0);
			}
		}
		cout << endl;
	}
	return;
}

vector<double> calculateAngle(int nrOfOutputs, vector<int> chainCode) {
	createChainCode(chainCode);

	// calculate vector size
	int pieceSize = chainCode.size() / nrOfOutputs;
	pieceSize++;

	// init vector
	vector<vector<int>> temp;
	vector<int> empty;
	empty.push_back(0);

	Point a;
	Point b;

	// initialize
	for (int index = 0; index < nrOfOutputs; index++) {
		temp.push_back(empty);
	}

	// sampling the chaincode
	for (int index = 0; index < chainCode.size(); index++) {
		temp.at(index / pieceSize).push_back(chainCode.at(index));
	}



	Point tempP;

	// create a vector of point for samplepoints
	vector<Point> points;
	points.push_back({ 0,0 });

	for (int index = 0; index < temp.size(); index++) {
		points.push_back(points.at((points.size() - 1)));
		for (int index1 = 1; index1 < temp.at(index).size(); index1++) {
			if (temp.at(index).at(index1) == 0) {
				points.at(points.size() - 1).x++;
			}
			if (temp.at(index).at(index1) == 1) {
				points.at(points.size() - 1).x++;
				points.at(points.size() - 1).y++;
			}
			if (temp.at(index).at(index1) == 2) {
				points.at(points.size() - 1).y++;
			}
			if (temp.at(index).at(index1) == 3) {
				points.at(points.size() - 1).x--;
				points.at(points.size() - 1).y++;
			}
			if (temp.at(index).at(index1) == 4) {
				points.at(points.size() - 1).x--;
			}
			if (temp.at(index).at(index1) == 5) {
				points.at(points.size() - 1).x--;
				points.at(points.size() - 1).y--;
			}
			if (temp.at(index).at(index1) == 6) {
				points.at(points.size() - 1).y--;
			}
			if (temp.at(index).at(index1) == 7) {
				points.at(points.size() - 1).x++;
				points.at(points.size() - 1).y--;
			}

		}

		// subtract two latest points
		tempP = points.at(points.size() - 1) - points.at(points.size() - 2);
	}

	// get angles
	vector<double> angles;
	for (int index = 0; index < points.size()-1; index++) {
		if (index == 0) {
			a = points.at(points.size() - 2) - points.at(0);
			b = points.at(1) - points.at(0);
			angles.push_back(GetAngleABC(a, b));
		}else{
			a = points.at(index -1) - points.at(index);
			b = points.at(index+1) - points.at(index);
			angles.push_back(GetAngleABC(a, b));
		}
	}

	double max = angles.at(distance(angles.begin(), max_element(angles.begin(), angles.end())));
	for (int index = 0; index < angles.size(); index++) {
		angles.at(index) /= max;
	}
	return angles;
}
	/*
	// calculate all the angles
	for (int index = 0; index < points.size()-1; index++) {
		angles.push_back(0);
		// check first angle
		if (index == 0) {
			tempP = points.at(points.size()-2)- points.at(index) ;
			if (tempP.x == 0 || tempP.y == 0) {
				if (tempP.x == 0) { angles.at(angles.size()-1 ) += 90; }
				if (tempP.y == 0) { angles.at(angles.size() - 1) += 90; }
			}
			else 
			{
				angles.at(angles.size() -1)  += abs(atan((double(tempP.y / (double)tempP.x))) * 180 / PI);
			}

			angles.at(angles.size() - 1) += abs(atan((double(tempP.x / (double)tempP.y))) * 180 / PI);

		}
		else 
		{	// not first angle

			// create vector
			tempP = points.at(index) - points.at(index - 1);

			// check devided by 0
			if (tempP.x == 0 || tempP.y == 0) {
				if (tempP.x == 0) { angles.at(angles.size() - 1) += (+90); }
				if (tempP.y == 0) { angles.at(angles.size() - 1) +=(+90); }
			}
			else
			{	// calculate angle
				angles.at(angles.size() - 1) += abs(atan((double(tempP.x / (double)tempP.y))) * 180 / PI);
			}





			// create vector
			tempP = points.at(index+1) - points.at(index);

			// check devided by 0
			if (tempP.x == 0 || tempP.y == 0) {
				if (tempP.x == 0) { angles.at(angles.size() - 1) += (+90); }
				if (tempP.y == 0) { angles.at(angles.size() - 1) += (+90); }
			}
			else 
			{	// calculate angle
				angles.at(angles.size()-1) += abs(atan((double(tempP.y / (double)tempP.x))) * 180 / PI);
			}
		}
	}

	double max = angles.at(distance(angles.begin(), max_element(angles.begin(), angles.end())));
	for (int index = 0; index < angles.size(); index++) {
		angles.at(index) /= max;
	}
	*/
	


void createChainCode(vector<int>& chainCode) {
	int d0 = 10;
	int d1 = 10;
	int d2 = 10;
	int d3 = 10;
	int d4 = 10;
	int d5 = 10;
	int d6 = 10;
	int d7 = 10;
	

	for (int index = 0; index < d0; index++) {
		chainCode.push_back(0);
	}
	for (int index = 0; index < d1; index++) {
		chainCode.push_back(1);
	}
	for (int index = 0; index < d2; index++) {
		chainCode.push_back(2);
	}
	for (int index = 0; index < d3; index++) {
		chainCode.push_back(3);
	}
	for (int index = 0; index < d4; index++) {
		chainCode.push_back(4);
	}
	for (int index = 0; index < d5; index++) {
		chainCode.push_back(5);
	}
	for (int index = 0; index < d6; index++) {
		chainCode.push_back(6);
	}
	for (int index = 0; index < d7; index++) {
		chainCode.push_back(7);
	}
	return;
}

double GetAngleABC(Point a, Point b)
{
	// calculate dot product
	return acos((a.x * b.x + a.y * b.y)/( sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2)))) * 180/PI;
}