// CodeJam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "string.h"
#include <algorithm>
#include <math.h>
#include <vector>

using namespace std;



int _tmain(int argc, _TCHAR* argv[])
{

	ifstream infile("D-small-attempt1.in");
	ofstream outfile("D-small-attempt1.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");
	//ifstream infile("B-large.in");
	//ofstream outfile("B-large.out");

	//memset(combos, NULL, 45*45*sizeof(unsigned long long));

	int N;
	infile >> N;
	
	for ( int kk = 0; kk < N; kk++ ){

		cout << "Case #" << kk+1 << ": ";
		outfile << "Case #" << kk+1 << ": ";

		int S;
		infile >> S;
		
		int X[101];
		int Y[101];
		int R[101];
	
		double cX = 0;
		double cY = 0;

		for ( int i = 0; i < S; i++ ){
			infile >> X[i];
			infile >> Y[i];
			infile >> R[i];
			cX += X[i];
			cY += Y[i];
		}
		cX /= (double(S));
		cY /= (double(S));
		
		double minRad;

		if ( S == 1){
			minRad = R[0];
		}
		if (S == 2){
			minRad = max(R[0], R[1]);
		}
		if (S == 3){
			double dist1 = (X[0]-X[1])*(X[0]-X[1])+(Y[0]-Y[1])*(Y[0]-Y[1]);
			double dist2 = (X[1]-X[2])*(X[1]-X[2])+(Y[1]-Y[2])*(Y[1]-Y[2]);
			double dist3 = (X[2]-X[0])*(X[2]-X[0])+(Y[2]-Y[0])*(Y[2]-Y[0]);
			dist1 = sqrt(dist1);
			dist2 = sqrt(dist2);
			dist3 = sqrt(dist3);
			dist1 += (R[0]+R[1]);
			dist2 += (R[1]+R[2]);
			dist3 += (R[2]+R[0]);
			minRad = min(min(dist1, dist2), dist3);
			minRad /= 2.0;
		}

		char nString[100];
		sprintf(nString, "%.20f", minRad);

		cout << nString << endl;
		outfile << nString << endl;

	}

	

	return 0;
}



