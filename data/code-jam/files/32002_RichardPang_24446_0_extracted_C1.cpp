#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

int m, n;
vector<vector<bool> > isbroken(m, vector<bool>(n, 0));

bool ison(int mask, int n) {
	return (1<<n) & mask;
}

bool ok(int k) {
	for (int i = 0; i < n - 1; i++) {
		if (ison(k, i) && ison(k, i + 1))
			return 0;
	}
	return 1;
}

bool valid(int k, int row) {
	for (int i = 0; i < n; i++) {
		if (ison(k, i) && isbroken[row][i])
			return 0;
	}
	return 1;
}

bool valid2(int prev, int next) {
	for (int i = 0; i < n; i++) {
		bool b = ison(prev, i);
		if (b && (i > 0) && ison(next, i - 1))
			return 0;
		if (b && (i < n - 1) && ison(next, i + 1))
			return 0;
	}
	return 1;
}

int size(int k) {
	int res = 0;
	while (k) {
		res += k & 1;
		k >>= 1;
	}
	return res;
}

int main() {
	ifstream fin("C-small-attempt0.in");
	ofstream fout("C-small.out");
	int c;
	fin >> c;
	for (int t = 0; t < c; t++) {
		fin >> m >> n;
		isbroken.clear();
		isbroken.resize(m, vector<bool>(n, 0));
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				char ch;
				fin >> ch;
				isbroken[i][j] = (ch == 'x');
			}
		}
		vector<int> possible;
		for (int i = 0; i < (1<<n); i++) {
			if (ok(i))
				possible.push_back(i);
		}
		vector<vector<int> > best(m, vector<int>((1<<n), 0));
		for (int i = 0; i < possible.size(); i++) {
			if (valid(possible[i], m - 1)) {
				best[m - 1][possible[i]] = size(possible[i]);
			}
		}
		for (int i = m - 2; i >= 0; i--) {
			for (int x = 0; x < possible.size(); x++) {
				for (int y = 0; y < possible.size(); y++) {
					if (valid(possible[x], i) && valid2(possible[x], possible[y])) {
						best[i][possible[x]] = max(best[i][possible[x]], size(possible[x]) + best[i + 1][possible[y]]);
					}
				}
			}
		}
		int res = 0;
		for (int i = 0; i < possible.size(); i++)
			res = max(res, best[0][possible[i]]);
		fout << "Case #" << t + 1 << ": " << res << endl;
	}
	return 0;
}
