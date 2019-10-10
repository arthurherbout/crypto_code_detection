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
ifstream fin("A-large.in");
ofstream fout("large.txt");

int N, C;

int main() {
	int t; fin >> t;
	for (int tt = 0; tt < t; tt++) {
		fin >> N; vector<int> v(N);
		for (int i = 0; i < N; i++) {
			string s; fin >> s;
			int k = -1;
			for (int j = 0; j < N; j++)
				if (s[j] == '1')
					k = j;
			v[i] = k;
		}

		int res = 0;
		for (int i = 0; i < N; i++) {
			int pos = -1;
			for (int j = i; j < N; j++)
				if (v[j] <= i) {
					pos = j;
					break;
				}
			for (int j = pos; j > i; j--) {
				swap(v[j], v[j - 1]);
				res++;
				}
		}
		fout << "Case #" << tt + 1 << ": " << res << endl;
	}
	return 0;
}
