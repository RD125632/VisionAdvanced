#include "stdafx.h"
#include "UnitTest.h"

// Loading functions
void UnitTest::loadTestFigures(vector<ImageObject> &testImages)
{
		for (int i = 0; i < 6; i++)
		{
			ostringstream oss;
			oss << "./../Images/NN_TEST/" << i << ".jpg";
			Mat image = imread(oss.str(), CV_LOAD_IMAGE_COLOR);

			//Original image -> grayscaled image
			Mat gray_image;
			cvtColor(image, gray_image, CV_BGR2GRAY);

			// Grayscaled image -> black/white scaled image
			Mat binary_image;
			threshold(gray_image, binary_image, 200, 1, CV_THRESH_BINARY_INV);

			Mat binary16S_image;
			binary_image.convertTo(binary16S_image, CV_16S);

			ImageObject IO = ImageObject(binary16S_image,0);
			testImages.push_back(IO);
		}
}
