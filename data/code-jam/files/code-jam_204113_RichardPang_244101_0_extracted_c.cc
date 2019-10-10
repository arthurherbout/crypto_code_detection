#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <iomanip>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
using namespace std;

//ifstream fin("input.txt"); 
ifstream fin("C-small-attempt2.in");
ofstream fout("Csmall.txt");

int N, K;

bool fits(vector<int>& v1, vector<int>& v2) {
	for (int i = 0; i < K; i++)
		if (v1[i] >= v2[i])
			return 0;
	return 1;
}

int main() {
	int t; fin >> t;
	for (int tt = 0; tt < t; tt++) {
		fin >> N >> K;
		vector<vector<int> >v(N, vector<int>(K));
		for (int i = 0; i < N; i++)
			for (int j = 0; j < K; j++)
				fin >> v[i][j];
		sort(v.begin(), v.end());

		vector<int> ok(N, 1);
		for (int i = 0; i < N; i++) {
			for (int j = i + 1; j < N; j++) {
				if (ok[j] && fits(v[i], v[j])) {
					ok[j] = 0;
					break;
				}
			}
		}
		int res = 0;
		for (int i = 0; i < N; i++)
			if (ok[i])
				res++;
		fout << "Case #" << tt + 1 << ": " << res << endl;
	}
	return 0;
}
