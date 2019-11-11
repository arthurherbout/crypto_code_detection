#include <fstream>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

typedef pair<int, int> pii;

int n, m;
vector<vector<pii> > v;

bool ok(int k) {
	for (int i = 0; i < m; i++) {
		bool ok = 0;
		for (int j = 0; j < v[i].size(); j++) {
			int r = k & (1<<(v[i][j].first - 1));
			r >>= v[i][j].first - 1;
			if (r == v[i][j].second)
				ok = 1;
		}
		if (!ok)
			return 0;
	}
	return 1;
}

int count(int k) {
	int res = 0;
	while (k) {
		res += k & 1;
		k >>= 1;
	}
	return res;
}

bool better(int a, int b) {
	if (a == -1)
		return b;
	if (count(a) < count(b))
		return a;
	else
		return b;
}

int main() {
	ifstream fin("B-small-attempt0.in");
	ofstream fout("B-small.out");
	int t;
	fin >> t;
	for (int c = 0; c < t; c++) {
		fin >> n >> m;
		v.resize(m);
		for (int i = 0; i < m; i++) {
			int k;
			fin >> k;
			v[i].resize(k);
			for (int j = 0; j < k; j++)
				fin >> v[i][j].first >> v[i][j].second;
		}
		int res = -1;
		for (int i = 0; i < (1<<n); i++) {
			if (ok(i))
				res = better(res, i);
		}

		fout << "Case #" << c + 1 << ": ";
		if (res == -1)
			fout << "IMPOSSIBLE" << endl;
		else {
			int k = res&1;
			fout << k;
			for (int i = 1; i < n; i++) {
				int kk = res & (1<<i);
				kk >>= i;
				fout << ' ' << kk;
			}
			fout << endl;
		}
	}
	return 0;
}