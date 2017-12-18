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

class UnitTest {
private:
public:
	void loadTestFigures(vector<ImageObject> &);
};




