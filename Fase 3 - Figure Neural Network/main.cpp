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
vector<int> genChainCode();

// Globals
vector<vector<ImageObject>> trainingImages;
vector<string> figureSets = { "cirkel", "vierkant", "driehoek" };
Figure_BPN bpn;
UnitTest unit;
Mat V0, W0;


int main(int argc, char** argv)
{
	initialize();

	vector<int> chaincode;
	chaincode = genChainCode();

	vector<ImageObject> testImages;
	unit.loadTestFigures(testImages);

	for (ImageObject iObject : testImages)
	{
		bpn.sortImage(iObject.getChainCode(), V0, W0);
	}
	//bpn.sortImage(trainingImages.at(0).at(0).getChainCode(), V0, W0);

	waitKey(0);
	return 0;
}



void initialize() {
	bpn.loadTrainingFigures(figureSets, trainingImages);
	bpn.train(trainingImages, V0 ,W0);
}


vector<int> genChainCode() {
	vector<int> chainCode;
	chainCode.push_back(30); // 0
	chainCode.push_back(0);  // 1
	chainCode.push_back(30); // 2
	chainCode.push_back(0);  // 3
	chainCode.push_back(30); // 4
	chainCode.push_back(0);  // 5
	chainCode.push_back(30); // 6
	chainCode.push_back(0);  // 7
	return chainCode;
}