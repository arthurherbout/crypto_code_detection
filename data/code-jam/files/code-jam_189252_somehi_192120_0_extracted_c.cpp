//#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <algorithm>
#include <math.h>


using namespace std;
using std::string;


ifstream fin("input.txt");
ofstream fout("output.txt");

int num;
int P;
int Q;
int q[100];
int qq[104];
int length;
int mat[104][104];	// (Q+2)x(Q+2)


int compopt(int j, int k)
{
	int i;
	int r;
	int min = 99999999;
	
	if (mat[j][k] > -1) {
		return mat[j][k];
	} else {
		if (j + 1 == k) {
			mat[j][k] = 0;
		} else {
			for (i = j+1; i < k; i++) {
				r = compopt(j, i) + compopt(i, k);
				if (r < min) {
					min = r;
				}
			}
			mat[j][k] = qq[k] - qq[j] - 2 + min;
		}
		return mat[j][k];
	}
}


int count()
{
	for (int j = 0; j < length; j++) {
		for (int k = 0; k < length; k++) {
			mat[j][k] = -1;
		}
	}
	
	int r = compopt(0, length-1);
	
	// for (int j = 0; j < length; j++) {
		// for (int k = 0; k < length; k++) {
			// cout << j << " - " << k << ": " << mat[j][k] << endl;
		// }
	// }
	
	return r;
}


int main()
{
	int i, j, k;
	
	fin >> num;
	
	for (i = 0; i < num; i++) {
		fin >> P >> Q;
		for (j = 0; j < Q; j++) {
			fin >> q[j];
		}
		
		qq[0] = 0;		
		
		for (k = 1; k < Q+1; k++) {
			qq[k] = q[k - 1];
		}
		
		qq[k] = P+1;
		
		length = Q+2;
		
		fout << "Case #" << i+1 << ": ";
		fout << count() << endl;
	}
	
	fin.close();
	fout.close();
	
	
	return 0;
}
