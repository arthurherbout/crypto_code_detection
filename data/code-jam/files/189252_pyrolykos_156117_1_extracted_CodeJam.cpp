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
	//ifstream infile("B-small-attempt0.in");
	//ofstream outfile("B-small-attempt0.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");
	ifstream infile("B-large.in");
	ofstream outfile("B-large.out");

	int N;
	infile >> N;

	for ( int k = 0; k < N; k++ ){

		cout << "Case #" << k+1 << ": ";
		outfile << "Case #" << k+1 << ": ";

		int numF;
		infile >> numF;

		int xSum = 0;
		int ySum = 0;
		int zSum = 0;
		int vxSum = 0;
		int vySum = 0;
		int vzSum = 0;

		int temp;

		for ( int i = 0; i < numF; i++ ){
			infile >> temp;
			xSum += temp;
			infile >> temp;
			ySum += temp;
			infile >> temp;
			zSum += temp;
			infile >> temp;
			vxSum += temp;
			infile >> temp;
			vySum += temp;
			infile >> temp;
			vzSum += temp;
		}

		long double x = ((long double)xSum)/((long double)numF);
		long double y = ((long double)ySum)/((long double)numF);
		long double z = ((long double)zSum)/((long double)numF);
		long double vx = ((long double)vxSum)/((long double)numF);
		long double vy = ((long double)vySum)/((long double)numF);
		long double vz = ((long double)vzSum)/((long double)numF);

		long double mag = vx*vx + vy*vy + vz*vz;
		long double numerator = -x*vx - y*vy - z*vz;

		long double tmin = 0;
		if (mag == 0){
			tmin = 0;
		}
		else {
			tmin = numerator / mag;
			if (tmin <= 0) tmin = 0;
		}

		double long minDist = (x+vx*tmin)*(x+vx*tmin)+(y+vy*tmin)*(y+vy*tmin)+(z+vz*tmin)*(z+vz*tmin);
		minDist = sqrt(minDist);

		char mdString[100];
		char tmString[100];
		sprintf(mdString, "%.20f", minDist);
		sprintf(tmString, "%.20f", tmin);

		//cout << minDist << " " << tmin << endl;
		//outfile << minDist << " " << tmin << endl;
		cout << mdString << " " << tmString << endl;
		outfile << mdString << " " << tmString << endl;

	}

	return 0;
}

