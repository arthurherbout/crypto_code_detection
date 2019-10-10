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

int comp(const void* a, const void* b)
{
	return (*((int *)(a)) < *((int*)(b)));
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream infile("B-small-attempt2.in");
	ofstream outfile("B-small-attempt2.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");

	int N;
	infile >> N;

	long long K;
	// remember if using getline, need to getline a trash
	// variable first
	int digits[24];
	int bigDigits[24];
	int numDigits = 0;
	int temp;
	for ( int k = 0; k < N; k++ ){
		cout << "Case #" << k+1 << ": ";
		outfile << "Case #" << k+1 << ": ";

		infile >> K;

		numDigits = 0;
		while (K > 0){
			temp = K % 10;
			digits[numDigits] = temp;
			bigDigits[numDigits] = temp;
			K /= 10;
			numDigits++;
		}


		int maxSoFar = digits[0];
		bool swap = false;
		for ( int i = 1; i < numDigits; i++ ){
			if (digits[i] < maxSoFar)
			{
				int positionToSwap = -1;
				int closestVal = 10;
				for (int j = i-1; j >= 0; j--){
					if ((digits[j] > digits[i])&&(digits[j] < closestVal)){
						closestVal = digits[j];
						positionToSwap = j;
					}
				}
				// now swap j and i
				int temp = digits[positionToSwap];
				digits[positionToSwap] = digits[i];
				digits[i] = temp;

				// now sort lowest digits
				qsort((void*)digits, i, sizeof(int), comp);
				swap = true;
				break;
			}
			if (digits[i] > maxSoFar) maxSoFar = digits[i];
		}

		if (!swap)
		{
			int min = 10;
			int pos = -1;
			for (int i = 0; i < numDigits; i++){
				if ((digits[i] < min)&&(digits[i] > 0)){
					min = digits[i];
					pos = i;
				}
			}
			int tempp = digits[pos];
			digits[pos] = 0;
			numDigits++;
			digits[numDigits-1] = tempp;
			qsort((void*)digits, numDigits-1, sizeof(int), comp);
		}

		long long newK = 0;
		long long exponent = 1;
		for ( int i = 0; i < numDigits; i++)
		{
			newK += digits[i]*exponent;
			exponent *= 10;
		}

		cout << newK << endl;
		outfile << newK << endl;

	}

	return 0;
}

