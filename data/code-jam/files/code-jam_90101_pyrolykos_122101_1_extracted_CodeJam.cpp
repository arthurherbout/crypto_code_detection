// CodeJam.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include "string.h"
#include <algorithm>
#include <math.h>

using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{
	//ifstream infile("A-small.in");
	//ofstream outfile("A-small.out");
	
	ifstream infile("C-large.in");
	ofstream outfile("C-large.out");

	//ifstream infile("testC.in");
	//ofstream outfile("test.out");

	int N;
	infile >> N;

	char garbage[10];
	infile.getline(garbage, 10);

	//int countArray[19*500];
	int countArray[19];

	char magicString[20] = "welcome to code jam";

	for ( int k = 0; k < N; k++ ){
		outfile << "Case #" << k+1 << ": ";
		
		char tempLine[505];
		infile.getline(tempLine, 505);

		// clear the array
		for ( int i = 0; i < 19; i++ ) countArray[i] = 0;

		for ( int i = strlen(tempLine)-1; i >= 0; i-- ){
			char c = tempLine[i];
			for ( int j = 0; j < 18; j++ ){
				if (magicString[j] == c){
					countArray[j] += countArray[j+1];
					if (countArray[j] >= 10000) countArray[j] = countArray[j] % 10000;
				}
			}
			if (magicString[18] == c){
					countArray[18]++;
					if (countArray[18] > 10000) countArray[18] -= 10000;
			}
		}
		
		char outputString[600];
		sprintf(outputString, "%04i", countArray[0]);

		outfile << outputString << endl;
	}

	return 0;
}

