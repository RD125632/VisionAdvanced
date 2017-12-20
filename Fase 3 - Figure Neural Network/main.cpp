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
void printEvaluation(Mat&);
bool validateInput(Mat&, Mat&);
void loadFactors();

// Globals
vector<vector<ImageObject>> trainingImages;
vector<ImageObject> objects;
vector<string> basicFigures = { "cirkel", "vierkant", "driehoek" };
vector<string> advancedFigures = {"hartje", "pentagram", "pijl", "rechthoeken", "ster" };
Figure_BPN bpn;
UnitTest unit;
Mat V0, W0, V1, W1;
int thresholdV = 30;

//
//int main(int argc, char** argv)
//{
//	initialize();
//	//loadFactors();
//	//vector<ImageObject> testImages;
//	//unit.loadTestFigures(testImages);
//
//	VideoCapture cap(1);
//
//	if (!cap.isOpened()){ 
//		cout << "error" << endl;
//		return -1; 
//	}
//
//	// Breedte en hooogte van de frames die de camera genereert ophalen. 
//	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
//	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
//	Mat image_filtered;
//	while (1)
//	{
//		cout << endl;
//		cout << "Plaats een object en druk op ENTER!!!!!" << endl;
//		cin.ignore();
//
//		Mat captured_image;
//		cap >> captured_image;
//		if (validateInput(captured_image, image_filtered))
//		{
//			Mat binairy_image1;
//			image_filtered.convertTo(binairy_image1, CV_16S);
//
//			evaluateObject(binairy_image1);
//		}
//	}
//
//	waitKey(0);
//	return 0;
//}
//

int main(int argc, char** argv)
{
	initialize();
	//loadFactors();
	//vector<ImageObject> testImages;
	//unit.loadTestFigures(testImages);

	Mat image = imread("./../Images/NN_TEST/0.jpg", CV_LOAD_IMAGE_COLOR);
	Mat grey_image;
	cvtColor(image, grey_image, CV_BGR2GRAY);

	Mat binary_image;
	threshold(grey_image, binary_image, thresholdV, 1, CV_THRESH_BINARY_INV);

	// Image Filtering
	Mat image_filtered;
	Mat kernel = (Mat_<int>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);
	cv::erode(binary_image, image_filtered, kernel);
	cv::dilate(image_filtered, binary_image, kernel);

	Mat binary16S_image;
	image_filtered.convertTo(binary16S_image, CV_16S);

	ImageObject IO = ImageObject(binary16S_image, 1);
	evaluateObject(binary16S_image);
	
	//show16SImageStretch(binary16S_image,"Used Image");

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

void loadFactors()
{
	FileStorage Factoren("Factoren.xml", FileStorage::READ);
	
	vector<Mat> trainings;
	FileNode k = Factoren["Features"];
	for (FileNodeIterator it = k.begin(); it != k.end(); ++it)
	{
		Mat feature;
		(*it) >> feature;
		trainings.push_back(feature);
	}

	V0 = trainings.at(0);
	W0 = trainings.at(1);
	V1 = trainings.at(2);
	W1 = trainings.at(3);

	Factoren.release();
}

void initialize() {
	bpn.loadTrainingFigures(basicFigures, trainingImages);
	bpn.train(trainingImages, V0 ,W0, 0);

	//bpn.loadTrainingFigures(advancedFigures, trainingImages);
	//bpn.train(trainingImages, V1, W1, 1);

	FileStorage Factoren("Factoren.xml", FileStorage::WRITE);
	Factoren << "Features" << "[";
	Factoren << V0;
	Factoren << W0;
	Factoren << V1;
	Factoren << W1;
	Factoren << "]";
	Factoren.release();
}

void evaluateObject(Mat binary_image)
{
	ImageObject IO = ImageObject(binary_image, 1);

	Mat evaluation;
	bpn.evaluateImage(IO.getChainCode(), V0, W0, evaluation);
	IO.calculateGeometry(evaluation);

	printEvaluation(evaluation);

	cout << endl<< evaluation << endl;

	//bpn.evaluateAdvImage(IO, V1, W1, evaluation);
	//cout <<endl << evaluation << endl;
}

void printEvaluation(Mat& evaluation)
{
	cout << endl;
	cout << "Object Evaluation:" << endl;
	cout << "----------------------------------------" << endl;
	cout << "Cirkel:		|" << roundf(evaluation.at<double>(0, 0) * 100) << "%" << endl;
	cout << "Vierkant:	|" << roundf(evaluation.at<double>(1, 0) * 100) << "%" << endl;
	cout << "Driehoek:	|" << roundf(evaluation.at<double>(2, 0) * 100) << "%" << endl;
	//cout << "Hartje:		|" << roundf(evaluation.at<double>(3, 0) * 100) << "%" << endl;
	//cout << "Pentagram:	|" << roundf(evaluation.at<double>(4, 0) * 100) << "%" << endl;
	//cout << "Pijl:		|" << roundf(evaluation.at<double>(5, 0) * 100) << "%" << endl;
	//cout << "Rechthoeken:	|" << roundf(evaluation.at<double>(6, 0) * 100) << "%" << endl;
	//cout << "Ster:		|" << roundf(evaluation.at<double>(7, 0) * 100) << "%" << endl;
	cout << "----------------------------------------" << endl;
}
