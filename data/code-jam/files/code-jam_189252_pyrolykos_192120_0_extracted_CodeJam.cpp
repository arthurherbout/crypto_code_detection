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

int runSim1(int* prisoners, int P, int a)
{
	return (P-1);
}
int runSim2(int* prisoners, int P, int a, int b)
{
	/*int answer = P-1;
	int bb = prisoners[b];
	int aa = prisoners[a];
	if (bb > aa){
		answer += bb-aa-1;
		answer += P-1-bb;
	}
	else{
		answer += aa-bb-1;
		answer += bb;
	}
	return answer;*/
	int answer = runSim1(prisoners, P, a);
	int aa = prisoners[a]-1;
	int bb = prisoners[b]-1;

	for (int i = bb; i >= 0; i--){
		if (i == aa) break;
		answer++;
	}
	for (int i = bb; i <= P-1; i++){
		if (i == aa) break;
		answer++;
	}

	return (answer-2);
}
int runSim3(int* prisoners, int P, int a, int b, int c)
{
	int answer = runSim2(prisoners, P, a, b);
	int aa = prisoners[a]-1;
	int bb = prisoners[b]-1;
	int cc = prisoners[c]-1;

	for (int i = cc; i >= 0; i--){
		if (i == aa) break;
		if (i == bb) break;
		answer++;
	}
	for (int i = cc; i <= P-1; i++){
		if (i == aa) break;
		if (i == bb) break;
		answer++;
	}
	return (answer-2);
}
int runSim4(int* prisoners, int P, int a, int b, int c, int d)
{
	int answer = runSim3(prisoners, P, a, b, c);
	int aa = prisoners[a]-1;
	int bb = prisoners[b]-1;
	int cc = prisoners[c]-1;
	int dd = prisoners[d]-1;

	for (int i = dd; i >= 0; i--){
		if (i == aa) break;
		if (i == bb) break;
		if (i == cc) break;
		answer++;
	}
	for (int i = dd; i <= P-1; i++){
		if (i == aa) break;
		if (i == bb) break;
		if (i == cc) break;
		answer++;
	}
	return (answer-2);
}
int runSim5(int* prisoners, int P, int a, int b, int c, int d, int e)
{
	int answer = runSim4(prisoners, P, a, b, c, d);
	int aa = prisoners[a]-1;
	int bb = prisoners[b]-1;
	int cc = prisoners[c]-1;
	int dd = prisoners[d]-1;
	int ee = prisoners[e]-1;

	for (int i = ee; i >= 0; i--){
		if (i == aa) break;
		if (i == bb) break;
		if (i == cc) break;
		if (i == dd) break;
		answer++;
	}
	for (int i = ee; i <= P-1; i++){
		if (i == aa) break;
		if (i == bb) break;
		if (i == cc) break;
		if (i == dd) break;
		answer++;
	}
	return (answer-2);
}
int _tmain(int argc, _TCHAR* argv[])
{
	ifstream infile("C-small-attempt1.in");
	ofstream outfile("C-small-attempt1.out");
	//ifstream infile("test.in");
	//ofstream outfile("test.out");
	//ifstream infile("B-large.in");
	//ofstream outfile("B-large.out");

	int N;
	infile >> N;

	int prisoners[110];

	for ( int kk = 0; kk < N; kk++ ){

		cout << "Case #" << kk+1 << ": ";
		outfile << "Case #" << kk+1 << ": ";

		int P;
		infile >> P;

		int Q;
		infile >> Q;

		int temp;
		for ( int i = 0; i < Q; i++){
			infile >> temp;
			prisoners[i] = temp;
		}

		int result = 0;

		if (Q == 1){
			result = runSim1(prisoners, P, 0);
		}
		if (Q == 2){
			result = runSim2(prisoners, P, 0, 1);
			temp = runSim2(prisoners, P, 1, 0);
			if (temp < result) result = temp;
		}
		if (Q == 3){
			result = runSim3(prisoners, P, 0, 1, 2);
			for (int i = 0; i < 3; i++){
				for (int j = 0; j < 3; j++){
					for (int k = 0; k < 3; k++){
						if ((k == j)||(k == i)||(i == j)) continue;
						temp = runSim3(prisoners, P, i, j, k);
						if (temp < result) result = temp;
					}
				}
			}
		}
		if (Q == 4){
			result = runSim4(prisoners, P, 0, 1, 2, 3);
			for (int i = 0; i < 4; i++){
				for (int j = 0; j < 4; j++){
					for (int k = 0; k < 4; k++){
						for (int l = 0; l < 4; l++){
						if ((k == j)||(k == i)||(i == j)) continue;
						if ((l == k)||(l == j)||(l == i)) continue;
						temp = runSim4(prisoners, P, i, j, k, l);
						if (temp < result) result = temp;
						}
					}
				}
			}
		}
		if (Q == 5){
			result = runSim5(prisoners, P, 0, 1, 2, 3, 4);
			for (int i = 0; i < 5; i++){
				for (int j = 0; j < 5; j++){
					for (int k = 0; k < 5; k++){
						for (int l = 0; l < 5; l++){
							for (int m = 0; m < 5; m++){
						if ((k == j)||(k == i)||(i == j)) continue;
						if ((l == k)||(l == j)||(l == i)) continue;
						if ((m == l)||(m == k)||(m == j)||(m==i)) continue;

						temp = runSim5(prisoners, P, i, j, k, l, m);
						if (temp < result) result = temp;
					}
				}
			}
		}
			}
		}

		cout << result << endl;
		outfile << result << endl;

	}

	return 0;
}

