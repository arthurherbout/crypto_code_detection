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
	ifstream infile("B-large.in");
	ofstream outfile("B-large.out");
	//ifstream infile("testB.in");
	//ofstream outfile("test.out");

	int N;
	infile >> N;

	int H, W;

	int oldmap[10000];
	char newmap[10000];


	for ( int k = 0; k < N; k++ ){
		outfile << "Case #" << k+1 << ":" << endl;

		infile >> H;
		infile >> W;

		for ( int i = 0; i < H; i++ ){
			for ( int j = 0; j < W; j++ ){
				infile >> oldmap[i*W+j];
				newmap[i*W+j] = '$';
				//cout << oldmap[i*W + j];
			}
			//cout << endl;
		}

		char whichBasin = 'a';
		for ( int i = 0; i < H; i++ ){
			for ( int j = 0; j < W; j++ ){
				int x = j;
				int y = i;
				bool keepGoing = true;
			
				vector<int> positionStack;
			while (keepGoing){
				int position = x+y*W;

				if (newmap[position] != '$'){
					// time to merge!
					char oldBasin = newmap[position];
					
					while (!positionStack.empty()){
						int index = positionStack.back();
						newmap[index] = oldBasin;
						positionStack.pop_back();
					}
					keepGoing = false;
					continue;
					//break;
				}

				positionStack.push_back(position);

				int lowestValue = 20000;
				int lowestIndexX = -1;
				int lowestIndexY = -1;
				int lowestIndex = -1;
				if ( y > 0 ){
					// check North
					if (oldmap[position-W] < lowestValue){
						lowestIndexX = x;
						lowestIndexY = y-1;
						lowestIndex = position-W;
						lowestValue = oldmap[lowestIndex];
					}
				}
				if ( x > 0 ){
					// check West
					if (oldmap[position-1] < lowestValue){
						lowestIndexX = x-1;
						lowestIndexY = y;
						lowestIndex = position-1;
						lowestValue = oldmap[lowestIndex];
					}
				}
				if ( x < W-1 ){
					// check East
					if (oldmap[position+1] < lowestValue){
						lowestIndexX = x+1;
						lowestIndexY = y;
						lowestIndex = position+1;
						lowestValue = oldmap[lowestIndex];
					}
				}
				if ( y < H-1 ){
					// check South
					if (oldmap[position+W] < lowestValue){
						lowestIndexX = x;
						lowestIndexY = y+1;
						lowestIndex = position+W;
						lowestValue = oldmap[lowestIndex];
					}
				}
				
				if (lowestValue >= oldmap[position]){
					// we've reached a new sink
					keepGoing = false;
					while (!positionStack.empty()){
						int index = positionStack.back();
						newmap[index] = whichBasin;
						positionStack.pop_back();
					}
					whichBasin++;
				}
				else{
					position = lowestIndex;
					x = lowestIndexX;
					y = lowestIndexY;
				}
			}
				//whichBasin++;
			}
		}

		for ( int i = 0; i < H; i++ ){
			for ( int j = 0; j < W; j++ ){
				outfile << newmap[i*W+j] << " ";
			}
			outfile << endl;
		}
	}

	return 0;
}

