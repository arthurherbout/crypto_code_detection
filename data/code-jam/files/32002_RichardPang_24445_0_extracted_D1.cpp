#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
	ifstream fin("D-small-attempt3.in");
	ofstream fout("D-small.out");
	int n;
	fin >> n;
	for (int t = 0; t < n; t++) {
		int h, w, r;
		fin >> h >> w >> r;
		vector<vector<int> > nways(h, vector<int>(w, 0));
		for (int i = 0; i < r; i++) {
			int rr, cc;
			fin >> rr >> cc;
			nways[rr - 1][cc - 1] = -1;
		}
		nways[0][0] = 1;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (nways[i][j] != -1) {
					if (i >= 1 && j >= 2 && nways[i - 1][j - 2] != -1)
						nways[i][j] = (nways[i][j] + nways[i - 1][j - 2]) % 10007;
					if (i >= 2 && j >= 1 && nways[i - 2][j - 1] != -1)
						nways[i][j] = (nways[i][j] + nways[i - 2][j - 1]) % 10007;
				}
			}
		}
		fout << "Case #" << t + 1 << ": " << nways[h - 1][w - 1] << endl;
	}
	return 0;
}
