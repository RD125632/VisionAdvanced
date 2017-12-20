#include "stdafx.h"
#include "BPN.h"

#define PI 3.14159265

// Loading functions
void Figure_BPN::loadTrainingFigures(vector<string> &figureSets, vector<vector<ImageObject>> &trainingImages)
{
	for (string figureSet : figureSets)
	{
		vector<ImageObject> figures;
		for (int i = 0; i < 10; i++)
		{
			ostringstream oss;
			oss << "./../Images/Figuren/" << figureSet << "/" << i << ".jpg";
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
			figures.push_back(IO);
		}
		trainingImages.push_back(figures);
	}
}

// Set creation
void Figure_BPN::createBPNOutput(int amountOfSets, int inSet, Mat &OTset)
{
	int totalTrainingInputImages = amountOfSets * inSet;
	OTset = (Mat_<double>(totalTrainingInputImages, amountOfSets));
	for (int index = 0; index < totalTrainingInputImages; index++) {
		for (int index1 = 0; index1 < inSet; index1++) {
			if (index1 == index / 10) {
				setEntry(OTset, index, index1, 1);
			}
			else {
				setEntry(OTset, index, index1, 0);
			}
		}
		cout << OTset << endl;
	}

	//switch (type) {
	//case 0:
	//	OTset = (Mat_<double>(30, 3) <<
	//		1, 0, 0,
	//		1, 0, 0,
	//		1, 0, 0,
	//		1, 0, 0,
	//		1, 0, 0,
	//		1, 0, 0,
	//		1, 0, 0, 
	//		1, 0, 0, 
	//		1, 0, 0, 
	//		1, 0, 0, //Cirkel
	//		0, 1, 0, 
	//		0, 1, 0,
	//		0, 1, 0,
	//		0, 1, 0,
	//		0, 1, 0, 
	//		0, 1, 0, 
	//		0, 1, 0, 
	//		0, 1, 0, 
	//		0, 1, 0, 
	//		0, 1, 0,  //Vierkant
	//		0, 0, 1, 
	//		0, 0, 1, 
	//		0, 0, 1, 
	//		0, 0, 1,
	//		0, 0, 1,
	//		0, 0, 1,
	//		0, 0, 1,
	//		0, 0, 1,
	//		0, 0, 1,
	//		0, 0, 1);
	//	break;
	//case 1:
	//	OTset = (Mat_<double>(80, 8) <<
	//		1, 0, 0, 0, 0, 0, 0, 0,
	//		1, 0, 0, 0, 0, 0, 0, 0,
	//		1, 0, 0, 0, 0, 0, 0, 0,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1,
	//		1, 0, 0, 0, 0, 0, 0, 1, //Cirkel
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1,
	//		0, 1, 0, 0, 0, 0, 0, 1, //Vierkant
	//		0, 0, 1, 0, 0, 0, 0, 0, 
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0, //Driehoek
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0,
	//		0, 0, 0, 1, 0, 0, 0, 0, //Hart
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0, //Penta
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0,
	//		0, 0, 1, 1, 0, 1, 0, 0, //Pijl
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0,
	//		0, 0, 1, 0, 0, 0, 0, 0, //Rechthoek
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0,
	//		0, 0, 0, 0, 0, 0, 0, 0); // Ster
	//	break;
	//}
}

void Figure_BPN::createBPNInput(vector<vector<ImageObject>> trainingImages, Mat &trainingSet) {

	Mat ITset = Mat_<double>(30, 9);

	int counter = 0;
	for (int o = 0; o < trainingImages.size(); o++)
	{
		for (ImageObject m : trainingImages.at(o))
		{
			ITset.at<double>(counter, 0) = 1;

			vector<double> unplotted = m.getChainCode();
			vector<double> plottedChainCode;
			for (int b = 0; b < 8; b++)
			{
				plottedChainCode.push_back(count(unplotted.begin(), unplotted.end(), b));
			}

			int max = distance(plottedChainCode.begin(), max_element(plottedChainCode.begin(), plottedChainCode.end()));
			double maxV = plottedChainCode.at(max);

			//Add chaincode to set
			for (int c = 0; c < 8; c++)
			{
				ITset.at<double>(counter, c + 1) = plottedChainCode.at(c) / maxV;
			}
			/*
			//Add angles to set
			vector<double> temp1 = calculateAngle(5, unplotted);
			for (int d = 8; d < 13; d++)  
			{	
				ITset.at<double>(counter, d + 1) = temp1.at(d-8);
			}*/

			//ITset.at<double>(counter, 9) = 1 / m.getContour().size();					// Omtrek
			//ITset.at<double>(counter, 9) = (int)m.getEquality();					// Gelijk figuur
			counter++;
		}
	}
	cout << ITset << endl;
	trainingSet = ITset;
}

void Figure_BPN::createBPNInput(vector<double> chainCode, Mat &trainingSet) 
{
	Mat ITset = Mat_<double>(9, 1);
	ITset.at<double>(0, 0) = 1;

	vector<double> plottedChainCode;
		
	for (int b = 0; b < 8; b++)
	{
		plottedChainCode.push_back(count(chainCode.begin(), chainCode.end(), b));
	}

	int max = distance(plottedChainCode.begin(), max_element(plottedChainCode.begin(), plottedChainCode.end()));
	double maxV = plottedChainCode.at(max);

	for (int c = 0; c < 8; c++)
	{
		ITset.at<double>(c+1, 0) = plottedChainCode.at(c) / maxV;
	}
	trainingSet = ITset;
}

void Figure_BPN::createBPNInput(ImageObject IO, Mat& evaluation, Mat &trainingSet)
{
	Mat ITset = Mat_<double>(8, 1);

	ITset.at<double>(0, 0) = 1;											// Bias
	ITset.at<double>(1, 0) = roundf(evaluation.at<double>(0, 0) * 100); // Cirkel
	ITset.at<double>(2, 0) = roundf(evaluation.at<double>(1, 0) * 100); // Vierkant
	ITset.at<double>(3, 0) = roundf(evaluation.at<double>(2, 0) * 100); // Driehoek
	ITset.at<double>(4, 0) = 1.0;										// Aantal hoeken
	ITset.at<double>(5, 0) = IO.getContour().size();					// Omtrek
	ITset.at<double>(6, 0) = IO.getSurface();							// Oppervlak
	ITset.at<double>(7, 0) = (int)IO.getEquality();						// Gelijk figuur

	trainingSet = ITset;
}

// func: Initialization of the (1) weigthmatrices V0 and W0 and (2) of the delta matrices dV0 and dW0. 
// pre: inputNeurons, hiddenNeurons and outputNeurons define the Neural Network. 
//      From this numbers the dimensions of the weightmatrices can be determined.
// post: V0 and W0 have random values between 0.1 and 0.9
void Figure_BPN::initializeBPN(int inputNeurons, int hiddenNeurons, int outputNeurons,
	Mat & V0, Mat & dV0, Mat & W0, Mat & dW0) {

	// Instellen van alle weegfactoren met een random waarde
	V0 = Mat_<double>(inputNeurons, hiddenNeurons);
	W0 = Mat_<double>(hiddenNeurons, outputNeurons);
	setRandomValue(V0, 0.1, 0.9);
	setRandomValue(W0, 0.1, 0.9);

	// Initiele aanpassing van de weegfactoren W
	dV0 = Mat_<double>(inputNeurons, hiddenNeurons);
	dW0 = Mat_<double>(hiddenNeurons, outputNeurons);
	setValue(dV0, 0);
	setValue(dW0, 0);
} // initializeBPN

void Figure_BPN::train(vector<vector<ImageObject>> &trainingImages, Mat& V0, Mat& W0, int nr)
{
	// V0, W0   : weightfactor matrices
	// dV0, dW0 : weightfactor correction matrices
	Mat dW0, dV0;

	// default number of hiddenNeurons. The definite number is user input  
	// inputNeurons and outputNeurons are implicitly determined via
	// the trainingset, i.e.: inputNeurons = ITset.cols ; outputNeurons = OTset.cols;
	int hiddenNeurons = 5;

	//cout << m << endl << endl;
	Mat ITS, OTS;
	createBPNInput(trainingImages,ITS);
	createBPNOutput(trainingImages.size(),10,OTS);

	// IT: current training input of the inputlayer 
	// OT: desired training output of the BPN
	// OH: output of the hiddenlayer
	// OO: output of the outputlayer
	Mat IT, OT, OH, OO;

	initializeBPN(ITS.cols, hiddenNeurons, OTS.cols, V0, dV0, W0, dW0);

	// outputError0: error on output for the current input and weighfactors V0, W0
	// outputError1: error on output for the current input and new calculated 
	//               weighfactors, i.e. V1, W1
	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
	Mat V1, W1;

	int runs = 0;
	while ((sumSqrDiffError > MAX_OUTPUT_ERROR) && (runs < MAXRUNS)) {
		sumSqrDiffError = 0;

		for (int inputSetRowNr = 0; inputSetRowNr < ITS.rows; inputSetRowNr++) {

			IT = transpose(getRow(ITS, inputSetRowNr));

			OT = transpose(getRow(OTS, inputSetRowNr));

			calculateOutputHiddenLayer(IT, V0, OH);

			calculateOutputBPN(OH, W0, OO);

			adaptVW(OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1);

			calculateOutputBPNError(OO, OT, outputError0);

			calculateOutputBPNError(BPN(IT, V1, W1), OT, outputError1);

			sumSqrDiffError += (outputError1 - outputError0) * (outputError1 - outputError0);

			V0 = V1;
			W0 = W1;
		}
		cout << "sumSqrDiffError = " << sumSqrDiffError << endl;
		runs++;
	}

	cout << "BPN Training is ready!" << endl << endl;
	cout << "Runs = " << runs << endl << endl;

	Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;

	// druk voor elke input vector uit de trainingset de output vector uit trainingset af 
	// tezamen met de output vector die het getrainde BPN (zie V0, W0) genereerd bij de 
	// betreffende input vector.
	cout << setw(10) << " Training Input" << setw(12) << "|" << " Expected Output "<< setw(1) << "|" << " Output BPN " << setw(6) << "|" << endl << endl;

	
	for (int row = 0; row < ITS.rows; row++) {

		// haal volgende inputvector op uit de training set
		inputVectorTrainingSet = transpose(getRow(ITS, row));

		// druk de inputvector af in een regel afgesloten met | 
		for (int r = 0; r < inputVectorTrainingSet.rows; r++) {
			cout << setw(10) << getEntry(inputVectorTrainingSet, r, 0);
		}
			
		cout << setw(2) << "|";

		// haal bijbehorende outputvector op uit de training set
		outputVectorTrainingSet = transpose(getRow(OTS, row));

		// druk de outputvector van de training set af in dezelfde regel afgesloten met | 
		for (int r = 0; r < outputVectorTrainingSet.rows; r++)
			cout << setw(8) << round(getEntry(outputVectorTrainingSet, r, 0));
		cout << setw(2) << "|";

		// bepaal de outputvector die het getrainde BPN oplevert 
		// bij de inputvector uit de trainingset  
		outputVectorBPN = BPN(inputVectorTrainingSet, V0, W0);

		// druk de output vector van het BPN af in dezelfde regel afgesloten met |--
		for (int r = 0; r < outputVectorBPN.rows; r++)
			cout << setw(8) << round(getEntry(outputVectorBPN, r, 0));
		cout << setw(2) << "|";
		cout << endl;
	}

	cout << endl << endl;
	cout << "V0" << endl;
	cout << V0 << endl << endl;
	cout << "W0" << endl << endl;
	cout << W0 << endl << endl;
}

void Figure_BPN::evaluateImage(vector<double> sortChaincode, Mat V0, Mat W0, Mat& evaluation) {
	Mat IT;
	Mat OH;
	Mat OO;
	createBPNInput(sortChaincode, IT);
	calculateOutputHiddenLayer(IT, V0, OH);
	calculateOutputBPN(OH, W0, OO);

	evaluation = OO;
}

void Figure_BPN::evaluateAdvImage(ImageObject IO, Mat V1, Mat W1, Mat& evaluation) {
	Mat IT;
	Mat OH;
	Mat OO;
	createBPNInput(IO, evaluation, IT);
	calculateOutputHiddenLayer(IT, V1, OH);
	calculateOutputBPN(OH, W1, OO);

	evaluation = OO;
}

vector<double> Figure_BPN::calculateAngle(int nrOfOutputs, vector<double> chainCode) {

	if (chainCode.size() % nrOfOutputs == 0) { nrOfOutputs--; }
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
	for (int index = 0; index < points.size() - 1; index++) {
		if (index == 0) {
			a = points.at(points.size() - 2) - points.at(0);
			b = points.at(1) - points.at(0);
			angles.push_back(GetAngleABC(a, b));
		}
		else {
			a = points.at(index - 1) - points.at(index);
			b = points.at(index + 1) - points.at(index);
			angles.push_back(GetAngleABC(a, b));
		}
	}

	double max = angles.at(distance(angles.begin(), max_element(angles.begin(), angles.end())));
	for (int index = 0; index < angles.size(); index++) {
		angles.at(index) /= max;
	}
	return angles;
}

double Figure_BPN::GetAngleABC(Point a, Point b)
{
	// calculate dot product
	return acos((a.x * b.x + a.y * b.y) / (sqrt(pow(a.x, 2) + pow(a.y, 2)) * sqrt(pow(b.x, 2) + pow(b.y, 2)))) * 180 / PI;
}
