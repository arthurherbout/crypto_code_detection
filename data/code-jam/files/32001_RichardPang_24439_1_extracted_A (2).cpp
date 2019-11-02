#include <set>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

int m, goal;
vector<int> types;
vector<int> canchange;
vector<int> vals;
map<int, int> store;

int solve(int k) {
	if (store.find(k) != store.end())
		return store[k];

	if (k >= (m - 1) / 2) {
		if (vals[k] == goal)
			return store[k] = 0;
		else
			return store[k] = 0xffff;
	}

	int res = 0xffff;
	if (goal == types[k]) {
		res = solve(2 * k + 1) + solve(2 * k + 2);
		if (canchange[k]) {
			res = min(res, 1 + min(solve(2 * k + 1), solve(2 * k + 2)));
		}
	} else {
		res = min(solve(2 * k + 1), solve(2 * k + 2));
	}
	return store[k] = res;
}

int main() {
//	ifstream fin("input.txt");
	ifstream fin("A-large.in");
	ofstream fout("A-large.out");

	int n;
	fin >> n;
	for (int t = 0; t < n; t++) {
		fin >> m >> goal;
		types.clear();
		canchange.clear();
		vals.resize(m);
		store.clear();

		for (int i = 0; i < m; i++) {
			if (i < (m - 1)/2) {
				int g, c;
				fin >> g >> c;
				types.push_back(g);
				canchange.push_back(c);
			} else {
				int k;
				fin >> k;
				vals[i] = k;
			}
		}

		int res = solve(0);
		fout << "Case #" << t + 1 << ": ";
		if (res >= 0xffff)
			fout << "IMPOSSIBLE" << endl;
		else
			fout << res << endl;
	}
	return 0;
}
