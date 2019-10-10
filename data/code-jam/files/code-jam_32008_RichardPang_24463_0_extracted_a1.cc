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

map<string, vector<string> > ing;
map<string, int> seen;

int solve(const string& s) {
	if (seen.find(s) != seen.end())
		return seen[s];
	if (ing[s].size() == 0)
		return seen[s] = 1;

	int used = 0;
	int extra = 0;
	for (int i = 0; i < ing[s].size(); i++) {
		int k = solve(ing[s][i]);
		if (k < extra)
			extra--;
		else
			extra = k - 1;
		used++;
	}
	if (extra >= used)
		extra -= used - 1;

	if (extra == 0)
		extra = 1;
	return seen[s] = used + extra;
}

int main() {
//	ifstream fin("input.txt");
	ifstream fin("A-small-attempt3.in");
	ofstream fout("output.txt");
	int nt;
	fin >> nt;
	for (int t = 0; t < nt; t++) {
		seen.clear();
		ing.clear();
		int n;
		fin >> n;
		vector<string> vs;
		for (int i = 0; i < n; i++) {
			string s;
			fin >> s;
			vs.push_back(s);
			int k;
			fin >> k;
			for (int j = 0; j < k; j++) {
				string t;
				fin >> t;
				if (isupper(t[0]))
					ing[s].push_back(t);
			}
		}
//		cout << endl;
		fout << "Case #" << t + 1 << ": " << solve(vs[0]) << endl;
	}
	return 0;
}
