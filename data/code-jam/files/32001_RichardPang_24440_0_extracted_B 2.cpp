#include <set>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

typedef pair<int, int> pii;

int main() {
//	ifstream fin("input.txt");
	ifstream fin("B-small-attempt0.in");
	ofstream fout("B-small.out");

	int c;
	fin >> c;
	for (int t = 0; t < c; t++) {
		int n, m, a;
		fin >> n >> m >> a;

		fout << "Case #" << t + 1 << ": ";
		if (a > n * m)
			fout << "IMPOSSIBLE" << endl;
		else {
			map<int, pii> mm;
			for (int i = 1; i <= n; i++)
				for (int j = 1; j <= m; j++)
					mm[i * j] = pii(i, j);

			if (mm.find(a) != mm.end())
				fout << "0 0 " << mm[a].first << " 1 0 " << mm[a].second << endl;
			else {
				bool found = 0;
				for (map<int, pii>::iterator it = mm.begin(); it != mm.end(); it++) {
					if (mm.find(a + it->first) != mm.end()) {
						pii p = mm[a + it->first];
						fout << "0 0 " << it->second.first << ' ' << p.second << ' ' << p.first << ' ' << it->second.second << endl;
						found = 1;
						break;
					}
				}
				if (!found)
					fout << "IMPOSSIBLE" << endl;
			}
		}
	}
	return 0;
}
