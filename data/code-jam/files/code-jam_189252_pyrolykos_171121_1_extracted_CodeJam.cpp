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
	ifstream infile("A-large.in");
	ofstream outfile("A-large.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");
	//ifstream infile("B-large.in");
	//ofstream outfile("B-large.out");

	int N;
	infile >> N;

	char garbage[100];
	infile.getline(garbage, 100);

	char line[100];

	char chars[40];

	int digits[66];

	for ( int k = 0; k < N; k++ ){

		cout << "Case #" << k+1 << ": ";
		outfile << "Case #" << k+1 << ": ";

		infile.getline(line, 100);
		int numChars = 0;

		for (int i = 0; i < strlen(line); i++){
			int pos = -1;
			for (int j = 0; j < numChars; j++){
				if (chars[j] == line[i]){
					pos = j;
					j = numChars+1;
					digits[i] = pos;
					if (pos == 0) digits[i] = 1;
					if (pos == 1) digits[i] = 0;
				}
			}
			if (pos == -1){
				numChars++;
				chars[numChars-1] = line[i];
				digits[i] = numChars-1;
				if (numChars-1 == 0) digits[i] = 1;
				if (numChars-1 == 1) digits[i] = 0;
			}
		}
		
		unsigned long long number = 0;
		unsigned long long exp = 1;

		if (numChars < 2) numChars = 2;

		// base is numChars
		for (int i = strlen(line)-1; i >= 0; i-- ){
			number += digits[i]*exp;
			exp *= (unsigned long long)(numChars);
		}


		// count the number of characters, this is our base
		// the earlier the character is, the smaller it is
		// (with the first character as a 1)

		cout << number << endl;
		outfile << number << endl;

	}

	return 0;
}

