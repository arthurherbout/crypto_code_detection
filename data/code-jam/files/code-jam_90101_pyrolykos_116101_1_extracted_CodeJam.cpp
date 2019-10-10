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

struct letter{
	letter* child[26];
	int numChildren[26];
};


int numMatches(string &pattern, letter* wordTree)
{
	if (pattern.length() == 0){
		return 1;
	}
	char c = pattern.at(0);
	if (c == '(')
	{
		int total = 0;
		int numSplits = pattern.find(')');
		for (int i = 1; i < numSplits; i++){
			c = pattern.at(i);
			int cIndex = (int)(c) - (int)('a');
			if (wordTree->numChildren[cIndex] > 0){
				total += numMatches(pattern.substr(numSplits+1, pattern.length()-numSplits-1), wordTree->child[cIndex]);
			}
		}
		return total;
	}
	else {
		int cIndex = (int)(c) - (int)('a');
		if (wordTree->numChildren[cIndex] <= 0) return 0;
		else return numMatches(pattern.substr(1, pattern.length()-1), wordTree->child[cIndex]);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	ifstream infile("A-large.in");
	ofstream outfile("A-large.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");

	int L;
	infile >> L;

	int D;
	infile >> D;

	int N;
	infile >> N;

	char tmp[20];
	char c;

	letter* wordList = new letter;

	for ( int k = 0; k < 26; k++) wordList->numChildren[k] = 0;

	for ( int i = 0; i < D; i++ ){
		infile >> tmp;
		
		letter * tmpList = wordList;
		for (int j = 0; j < L; j++){
		    c =	tmp[j];
			int cIndex = (int)(c)-(int)('a');
			if (tmpList->numChildren[cIndex] > 0){
				tmpList->numChildren[cIndex]++;
				tmpList = tmpList->child[cIndex];
			}
			else {
				tmpList->numChildren[cIndex]++;
				tmpList->child[cIndex] = new letter;
				tmpList = tmpList->child[cIndex];
				for (int k = 0; k < 26; k++) tmpList->numChildren[k] = 0;
			}
		}
	}

	char ptm[1000];
	for ( int k = 0; k < N; k++ ){
		outfile << "Case #" << k+1 << ": ";
		
		infile >> ptm;
		string newString = string(ptm);

		outfile << numMatches(newString, wordList) << endl;
	}

	return 0;
}

