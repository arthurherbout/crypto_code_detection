#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
using namespace std;

string perm(const string& s, const vector<int>& v) {
	string res = s;
	for (int k = 0; k < s.length() / v.size(); k++) {
		for (int i = 0; i < v.size(); i++) {
			res[i + k * v.size()] = s[v[i] + k * v.size()];
		}
	}
	return res;
}

int val(const string& s) {
	int res = 1;
	for (int i = 1; i < s.length(); i++)
		if (s[i] != s[i - 1])
			res++;
	return res;
}

int main() {
	ifstream fin("D-small-attempt0.in");
	ofstream fout("D-small.out");

	int n;
	fin >> n;
	for (int t = 0; t < n; t++) {
		int k;
		fin >> k;
		string s;
		fin >> s;

		vector<int> v;
		for (int i = 0; i < k; i++)
			v.push_back(i);

		int res = s.length();
		do {
			string t;
			t = perm(s, v);
			res = min(res, val(t));
		} while (next_permutation(v.begin(), v.end()));
		fout << "Case #" << t + 1 << ": " << res << endl;
	}
	return 0;
}
