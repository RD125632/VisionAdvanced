/*  Functionaliteit: Werking van een Neuron (Perceptron)

W1*I1 + w2*I2 = input

input > threshold = > output = 1

input < threshold = > output = 0

aanpassing w1, w2 en threshold op basis van een gelabeld paar(I1, I2, correctoutput)

aanpassing threshold = -(correctoutput � output)

aanpassing W1 = aanpassing threshold * I1

aanpassing W2 = aanpassing threshold * I2

Jan Oostindie 
Avans Hogeschool 
Breda
email: jac.oostindie@avanns.nl
dd 9-9-2016
*/

#include "stdafx.h"
#include <iostream>
#include <iomanip>

using namespace std;

// Source: http://www.ee.surrey.ac.uk/Projects/CAL/digital-logic/gatesfunc/
//Truth tables

//AND Gate
//|A	|B	|A,B
//|0	|0	|0
//|0	|1	|0
//|1	|0	|0
//|1	|1	|1
//OR Gate
//|A	|B	|A,B
//|0	|0	|0
//|0	|1	|1
//|1	|0	|1
//|1	|1	|1
//AND x1,x2 OR x3
//|x1	|x2		|x3		|OUTPUT
//|0	|0		|0		|0
//|0	|0		|1		|1
//|0	|1		|0		|0
//|0	|1		|1		|1
//|1	|0		|0		|0
//|1	|0		|1		|1
//|1	|1		|0		|1
//|1	|1		|1		|1

double Gate[8][4] = {
	{ 0.0, 0.0, 0.0 , 0.0 },
	{ 0.0, 0.0, 1.0 , 1.0 },
	{ 0.0, 1.0, 0.0 , 0.0 },
	{ 0.0, 1.0, 1.0 , 1.0 },
	{ 1.0, 0.0, 0.0 , 0.0 },
	{ 1.0, 0.0, 1.0 , 1.0 },
	{ 1.0, 1.0, 0.0 , 1.0 },
	{ 1.0, 1.0, 1.0 , 1.0 }
};

// Start threshold
double threshold = 185;

/**
	@desc	Find all contours in binary_image
	@param	- Table cols		{ x1, x2 }  
			- Weight factors	{ W1, W2 }  
			- Threshold			{ threshold }
	@return perceptron output
*/
double perceptronOutput(double, double, double, double, double, double, double);
/*
	@desc	Print 
*/
void printBillGates();

void main() {
	// startwaarden van de weegfactoren (willekeurig)
	double W1 = 1003;
	double W2 = 456;
	double W3 = 501;

	// Output variabele
	double output, deltaOutput;

	// aanpassingen
	double deltaThreshold = 0.0, deltaW1 = 0.0, deltaW2 = 0.0, deltaW3 = 0.0;

    // boolean flag wordt gehezen als output meer dan EPSILON afwijkt van verwachte output
	bool flag = false;
	const int EPSILON = 0.000001;
	
	// Waarheidstabel afdrukken 
	printBillGates();

	int aantalRuns = 0;
	do {
		flag = false;
		for (int rij = 0; rij < 8; rij++) {
			// bereken actuele output van het nog ongetrainde perceptron
			output = perceptronOutput(Gate[rij][0], Gate[rij][1], Gate[rij][2], W1, W2, W3, threshold);

			// bereken de afwijking van de actuele output t.o.v. verwachte output
			deltaOutput = (output - Gate[rij][3]);

			if (abs(deltaOutput) > EPSILON) {
				
				flag = true;

				// berekening aanpassing W1 en W2
				deltaW1 = -deltaOutput*Gate[rij][0];
				deltaW2 = -deltaOutput*Gate[rij][1];
				deltaW3 = -deltaOutput*Gate[rij][2];

				// threshold, W1 en W2 aanpassen
				threshold += deltaOutput;
				W1 += deltaW1;
				W2 += deltaW2;
				W3 += deltaW3;
			} // if

		} // for

		cout << "Runs " << ++aantalRuns << 
			"   Threshold = " << threshold << endl;
	}
	while (flag);

	cout << "Berekende threshold = " << threshold << endl;
	cout << "Berekende W1 = " << W1 << endl;
	cout << "Berekende W2 = " << W2 << endl;
	cout << "Berekende W3 = " << W3 << endl;
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

	// genereer waarheidstabel met getrainde perceptron
	cout << "waarheidstabel gegenereerd met perceptron => " << endl;
	for (int rij = 0; rij < 8; rij++) {
		cout << setw(3) << Gate[rij][0] << setw(3) << Gate[rij][1] << setw(3) << Gate[rij][2] << setw(3)
			 << perceptronOutput(Gate[rij][0], Gate[rij][1], Gate[rij][2], W1, W2, W3, threshold) << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();

} // main
		
double perceptronOutput(double x1, double x2, double x3, double W1, double W2, double W3, double threshold) {
	// bereken input
	double input = W1*x1 + W2*x2 + W3*x3;

	// bepaal output
	if (input > threshold) return 1.0;
	else return 0.0;
} // perceptronOutput

void printBillGates()
{
	cout << "Waarheidstabel waarvoor het perceptron wordt getraind" << endl;
	// AND Gate 
	for (int rij = 0; rij < 8; rij++) {
		cout << setw(3) << Gate[rij][0] << setw(3) << Gate[rij][1] << setw(3) << Gate[rij][2] << setw(3) << Gate[rij][3] << endl;
		cout << endl;
	}
	cout << "Press ENTER to continue" << endl;
	cin.ignore();
}
