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

using namespace std;
using namespace cv;

void initialize();
void evaluateObject(Mat);

// Globals
vector<vector<ImageObject>> trainingImages;
vector<string> figureSets = { "cirkel", "vierkant", "driehoek" };
Figure_BPN bpn;
UnitTest unit;
Mat V0, W0;


int main(int argc, char** argv)
{
	//initialize();

	//vector<ImageObject> testImages;
	//unit.loadTestFigures(testImages);

	VideoCapture cap(1);

	// Controle of de camera wordt herkend.
	if (!cap.isOpened())
	{
		cout << "Cannot open the video cam" << endl;
		return -1;
	}

	// Breedte en hooogte van de frames die de camera genereert ophalen. 
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;

	// Window maken waarin de beelden "live" getoond worden
	//namedWindow("MyVideo", CV_WINDOW_AUTOSIZE); 

	// Continue loop waarin een beeld wordt opgehaald en wordt getoond in het window
	Mat frame;
	int thresholdV = 30;
	while (1)
	{
		cout << "Plaats een object en druk op ENTER!!!!!" << endl;
		cin.ignore();



		// Lees een nieuw frame
		bool bSuccess = cap.read(frame);

		// Controlleer of het frame goed gelezen is.
		if (!bSuccess)
		{
			cout << "Cannot read a frame from video stream" << endl;
			break;
		}
	
		Mat grey_image;
		cvtColor(frame, grey_image, CV_BGR2GRAY);
		//imshow("MyVideo", grey_image);
		Mat binary_image;
		threshold(grey_image, binary_image, thresholdV, 1, CV_THRESH_BINARY_INV);
		
		// Image Filtering
		Mat image_filtered;
		Mat kernel = (Mat_<int>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
		//cv::erode(binary_image, image_filtered, kernel);
		cv::erode(binary_image, image_filtered, kernel);
		cv::dilate(image_filtered, binary_image, kernel);
		//cv::erode(image_filtered, binary_image, kernel);
		imshow("Filtered", image_filtered*255);
		waitKey(1);

		Mat binairy_image1;
		image_filtered.convertTo(binairy_image1, CV_16S);

		evaluateObject(binairy_image1);
		waitKey(1);

	}
	waitKey(0);
	return 0;
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
