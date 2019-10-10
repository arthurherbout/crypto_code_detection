#include <map>
#include <set>
#include <queue>
#include <vector>
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
typedef vector<vi> vvi;

map<pair<vector<vector<int> >, pii>, int> seen;
int r, c;

bool solve(int rr, int cc, vvi v) {
	if (seen.find(make_pair(v, pii(rr, cc))) != seen.end())
		return seen[make_pair(v, pii(rr, cc))];
	v[rr][cc] = 0;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (rr + i < 0 || rr + i >= r || cc + j < 0 || cc + j >= c)
				continue;
			if (i == 0 && j == 0)
				continue;
			if (v[rr + i][cc + j] == 0)
				continue;
			if (!solve(rr + i, cc + j, v)) {
				return seen[make_pair(v, pii(rr, cc))] = 1;
			}
		}
	}
	return seen[make_pair(v, pii(rr, cc))] = 0;
}

int main() {
	ifstream fin("D-small-attempt4.in");
//	ifstream fin("input.txt");
	ofstream fout("output.txt");
	int n;
	fin >> n;
	for (int t = 0; t < n; t++) {
		seen.clear();
		fin >> r >> c;
		int kr = -1, kc = -1;
		vector<vector<int> > v(r, vector<int>(c));
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				char ch;
				fin >> ch;
				if (ch == 'K') {
					kr = i;
					kc = j;
					v[i][j] = 1;
				} else if (ch == '#')
					v[i][j] = 0;
				else
					v[i][j] = 1;
			}
		}
		bool b = solve(kr, kc, v);
		fout << "Case #" << t + 1 << ": ";
		if (b)
			fout << 'A' << endl;
		else
			fout << 'B' << endl;
	}
	return 0;
}
