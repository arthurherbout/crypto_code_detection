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

	ifstream infile("A-small-attempt2.in");
	ofstream outfile("A-small-attempt2.out");
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

		char row[50];
		char garbage[50];

		int keyNums[50];

		bool swaps[50];
		for (int i = 0; i < 50; i++) swaps[i] = false;

		infile.getline(garbage, 49);

		for ( int i = 0; i < S; i++ ){
			infile.getline(row, 49);
			keyNums[i] = 0;
			for (int j = 0; j < S; j++ ){
				if (row[j] == '1'){
					keyNums[i] = j;
				}
			}
		}

		// now just deal with keyNum switches
		int numSwaps = 0;
		int temp;

		/*for ( int i = 0; i < S; i++)
		{
			for ( int j = i; j < S; j++){
				if (keyNums[j] <= i){
					// swap until it's at the top
					numSwaps += j-i;
					temp = keyNums[j];
					for (int k = j; k > 0; k-- ){
						keyNums[k] = keyNums[k-1];
					}
					keyNums[0] = temp;
					
					//for ( int u = 0; u < S; u++ ) cout << keyNums[u] << " ";
					//cout << endl;
				}
			}
		}
*/
		bool swapAgain = true;
		bool wasSwapped = false;
		int pos = -1;

		int history[10000];

		while (swapAgain){
		swapAgain = false;
		wasSwapped = false;
		for ( int i = 0; i < S-1; i++ ){
			if ( keyNums[i] > i ){
				swapAgain = true;
				pos = i;
				if ( keyNums[i] > keyNums[i+1] ){
					wasSwapped = true;
					history[numSwaps] = i;
					numSwaps++; 
					
					temp = keyNums[i];
					keyNums[i] = keyNums[i+1];
					keyNums[i+1] = temp;
				}
			}
		}
		if (!wasSwapped){
			for ( int i = pos; i < S-1; i++ ){
				if ( keyNums[i] > keyNums[i+1] ){
					history[numSwaps] = i;
					numSwaps++;
					temp = keyNums[i];
					keyNums[i] = keyNums[i+1];
					keyNums[i+1] = temp;
					break;
				}
			}
		}
		}
//*/

		for ( int i = 0; i < numSwaps-2; i++ ){
			if (history[i] == history[i+2]) numSwaps--;
		}

	cout << numSwaps << endl;
	outfile << numSwaps << endl;

	}

	

	return 0;
}



