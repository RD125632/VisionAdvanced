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
void printEvaluation(Mat&);

// Globals
vector<vector<ImageObject>> trainingImages;
vector<string> figureSets = { "cirkel", "vierkant", "driehoek" };
Figure_BPN bpn;
UnitTest unit;
Mat V0, W0;


int main(int argc, char** argv)
{
	initialize();

	vector<ImageObject> testImages;
	unit.loadTestFigures(testImages);

	Mat evaluation;

	for (ImageObject iObject : testImages)
	{
		bpn.evaluateImage(iObject.getChainCode(), V0, W0, evaluation);
		printEvaluation(evaluation);
	}
	//bpn.evaluateImage(trainingImages.at(0).at(0).getChainCode(), V0, W0);

	waitKey(0);
	return 0;
}

void initialize() {
	bpn.loadTrainingFigures(figureSets, trainingImages);
	bpn.train(trainingImages, V0 ,W0);
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