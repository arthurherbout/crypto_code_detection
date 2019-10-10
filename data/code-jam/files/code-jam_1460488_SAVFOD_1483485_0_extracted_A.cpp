// A.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <tchar.h>

using namespace std;


int main(int argc, _TCHAR* argv[])
{	
	//FILE* out = fopen("out.txt", "w");
	char table[26];

	char buffer[1000];
	char secondBuffer[1000];
	ifstream rules("rules.txt", ifstream::in);
	rules.getline(buffer, 1000);
	rules.getline(secondBuffer, 1000);

	for (int i = 0; i < 26; ++i) {
		table[i] = '!';
	}

	for (int i = 0; buffer[i] != NULL; ++i) {
		table[buffer[i] - 'a'] = secondBuffer[i];
		//table[25] = 'q';
		//table['o' - 'a'] = 'e';
		//table[16] = 'z';
	}
	table['z' - 'a'] = 'q';
	table['q' - 'a'] = 'z';

	
	ifstream inFile("in.txt", ifstream::in);
	int count; 
	inFile >> count;
	inFile.getline(buffer, 1000);
	ofstream outFile("out.txt", ifstream::in);

	for (int line = 0; line < count; ++line) {
		inFile.getline(buffer, 1000);
		int i;
		for (i = 0; buffer[i] != NULL; ++i) {
			secondBuffer[i] = table[buffer[i] - 'a'];
		}
		secondBuffer[i] = NULL;
		outFile << "Case #" << line + 1 << ":" << " ";
		outFile << secondBuffer << endl;
	}

	outFile.close();
	return 0;
}

