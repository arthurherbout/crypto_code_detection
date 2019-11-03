#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include "fstream"
#include "string.h"

using namespace std;

int main (void)
{
	ifstream infile("A-large.in");
	ofstream outfile("A-large.out");

	int T;
	infile >> T;

	for (int i = 0; i < T; i++)
	{
		double A, B;
		infile >> A;
		infile >> B;

		double p[(int)A];

		for (int j = 0; j < A; j++)
		{
			infile >> p[j];
		}

		double bestCost = B+2; // just pressing enter
		double penalty = B+1;

		double goodSoFar = 1;
		for (int j = 0; j < A; j++)
		{
			double numDeletes = A-j;
			double bestScore = (B-A+1+numDeletes+numDeletes);
			double newCost = goodSoFar*(bestScore) + (1-goodSoFar)*(bestScore + penalty);
			if (newCost < bestCost)
				bestCost = newCost;

			goodSoFar = goodSoFar*p[j];
		}

		double numDeletes = 0;
		double bestScore = (B-A+1);
		double newCost = goodSoFar*(bestScore) + (1-goodSoFar)*(bestScore + penalty);
		if (newCost < bestCost)
			bestCost = newCost;

		// need probability that at least one of first N chars is bad
		cout << "Case #" << (i+1) << ": " << setiosflags(ios::fixed) << setprecision(6) << bestCost << endl;

		outfile << "Case #" << (i+1) << ": " << setiosflags(ios::fixed) << setprecision(6) << bestCost << endl;
//		outfile << "Case #" << (i+1) << ": " << fixed << setprecision(6) << bestCost << resetiosflags(ios::fixed) << endl;

//	outfile << "Case #" << (i+1) << ": " << bestCost << endl;

	}

	outfile.close();

	return 0;
}
