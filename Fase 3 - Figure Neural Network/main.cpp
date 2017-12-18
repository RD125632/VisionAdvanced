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


// Globals
vector<vector<ImageObject>> trainingImages;
vector<string> figureSets = { "cirkel", "vierkant", "driehoek" };
Figure_BPN bpn;
UnitTest unit;

int main(int argc, char** argv)
{
	//bpn.loadTrainingFigures(figureSets, trainingImages);
	//bpn.train(trainingImages);
	
	vector<ImageObject> testImages;
	unit.loadTestFigures(testImages);
	//cout << testImages.at(0).getImage() << endl;
	show16SImageStretch(testImages.at(0).getImage(), "16s");
	
	waitKey(0);
	return 0;
}