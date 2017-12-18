#pragma once

#include "stdafx.h"
#include "ImageObject.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;
using namespace cv;

class Figure_BPN {
private:
	// Maximale fout die toegestaan wordt in de output voor de training input
	const double MAX_OUTPUT_ERROR = 1E-10;

	// maximaal aantal runs dat uitgevoerd wordt bij het trainen
	const int MAXRUNS = 10000;
public:
	void loadTrainingFigures(vector<string> &, vector<vector<ImageObject>> &);
	void createBPNOutput(int, Mat &);
	void createBPNInput(vector<vector<ImageObject>>, Mat &);
	void initializeBPN(int, int, int, Mat &, Mat &, Mat &, Mat &);
	void train(vector<vector<ImageObject>> &, Mat&, Mat&);
	void sortImage(vector<int>, Mat, Mat);
};




