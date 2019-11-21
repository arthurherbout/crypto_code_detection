#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <set>
using namespace std;

ifstream fin("A-small-attempt0.in");
ofstream fout("output.txt");

typedef vector<int> vi;
set<int> good, bad;

int next(int val, int base) {
	int res = 0;
	while (val) {
		res += (val % base) * (val % base);
		val /= base;
	}
	return res;
}

bool goods(int val, int base) {
	if (good.find(val) != good.end())
		return 1;
	if (bad.find(val) != bad.end())
		return 0;

	set<int> seen;
	while (1) {
		val = next(val, base);
		if (val == 1) {
			good.insert(val);
			return 1;
		}

		if (seen.find(val) != seen.end())
			break;
		seen.insert(val);
	}
	for (set<int>::iterator it = seen.begin(); it != seen.end(); ++it)
		bad.insert(*it);

	return 0;
}

int main() {
	int t;
	fin >> t;
	string line; getline(fin, line);
	for (int tt = 0; tt < t; tt++) {
		bad.clear();
		good.clear();
		getline(fin, line);
		vi bases; int k;
		stringstream ss(line);
		while (ss >> k)
			bases.push_back(k);
		
		int res = 1;
		while (1) {
			res++;
			bool ok = 1;
			for (int i = 0; i < bases.size(); i++)
				if (!goods(res, bases[i])) {
					ok = 0;
					break;
				}
			if (ok)
				break;
		}
		fout << "Case #" << tt + 1 << ": " << res << endl;
	}
	return 0;
}