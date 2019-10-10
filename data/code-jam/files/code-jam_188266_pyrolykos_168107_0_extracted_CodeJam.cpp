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

bool notHappy(int base, int num, int pathLength, int* trail)
{
	int curNum = num;
	trail[pathLength] = curNum;
	

	int digit;
	int digitSum = 0;
	while (num > 0){
		digit = num % base;
		num /= base;
		digit *= digit;
		digitSum += digit;
	}
	
	//if (digitSum < base) return (digitSum != 1);
	if (digitSum == 1) return false;

	for ( int i = 0; i < pathLength; i++ ){
		if (trail[i] == curNum) return true;
	}

	return notHappy(base, digitSum, pathLength+1, trail);
}


int _tmain(int argc, _TCHAR* argv[])
{
	//ifstream infile("A-small-attempt0.in");
	//ofstream outfile("A-small-attempt0.out");
	ifstream infile("A-small-attempt1.in");
	ofstream outfile("A-small-attempt1.out");

	int N;
	infile >> N;

	int bases[10];
	string tempBase = "";

	char curString[500];

	char gString[500];

	int myInts[10];
infile.getline(gString, 100);

	int trail[10000];

	for ( int k = 0; k < N; k++ ){

		outfile << "Case #" << k+1 << ": ";
		
		
		infile.getline(curString, 100);
		int i = 0;
		int j = 0;
		while (i < strlen(curString)){
		tempBase = "";
		while ((curString[i] != ' ')&&(i < strlen(curString)))
		{
			tempBase += curString[i];
			i++;
		}

		bases[j] = atoi(tempBase.c_str());

		j++;
		i++;
		}

		int t = 2;
		bool check = true;
		while ( check ){
			check = false;
			for ( int l = 0; l < j; l++ ){
				if ( notHappy( bases[l], t, 0, trail ) ){
					check = true;
					l = j+1;
				}
			}
			t++;
		}
		outfile << t-1 << endl;
		cout << "Answer: " << t-1 << endl;
	}

	return 0;
}

