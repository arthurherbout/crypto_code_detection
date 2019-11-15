#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

void readData() {
}

void rec(const string &s, const string &t, int i, int j, string res, int state, vector < string > &out) {
	if (i == s.size() && j == t.size()) {
		out.push_back(res);
		return;
	}

	if (i < s.size()) {
		if (s[i] == '?') {
			rec(s, t, i + 1, j, res + string(1, state + '0'), state, out);
		} else {
			rec(s, t, i + 1, j, res, s[i] - '0', out);
		}
	}

	if (j < t.size()) {
		if (t[j] == '?') {
			rec(s, t, i, j + 1, res + string(1, state + '0'), state, out);
		} else {
			rec(s, t, i, j + 1, res, t[j] - '0', out);
		}
	}
}

vector < string > gen(string s1, string s2) {
	vector < string > out;
	rec(s1, s2, 0, 0, "", 0, out);
	sort(out.begin(), out.end());
	out.resize(unique(out.begin(), out.end()) - out.begin());
	return out;
}

void generate(int maxLen, int len, string cur, vector < string > &out) {
	if (cur.size() > 0) {
		out.push_back(cur);
	}
	if (len == maxLen) {
		return;
	}

	generate(maxLen, len + 1, cur + string(1, '0'), out);
	generate(maxLen, len + 1, cur + string(1, '1'), out);
	generate(maxLen, len + 1, cur + string(1, '?'), out);
}

void solve(int test) {
	int n, l;
	cin >> n >> l;
	vector < string > s(n);
	for (int i = 0; i < n; ++i) {
		cin >> s[i];
	}
	string b;
	cin >> b;
	sort(s.begin(), s.end());
	if (s.size() > 0 && s.back() == string(l, '1')) {
		printf("Case #%d: IMPOSSIBLE\n", test);
		return;
	}

	string s1 = "", s2(l - 1, '1');
	for (int i = 0; i < l; ++i) {
		s1 += "0?";
	}
	if (s2 == "") {
		s2 += "0";
	}
	printf("Case #%d: %s %s\n", test, s1.c_str(), s2.c_str());
}

void precalc() {
	vector < string > s;
	generate(5, 0, "", s);
	pair < string, string > f[50];
	for (int i = 0; i < s.size(); ++i) {
		cerr << i << endl;
		for (int j = i; j < s.size(); ++j) {
			int total = count(s[i].begin(), s[i].end(), '?') + count(s[j].begin(), s[j].end(), '?');
			vector < string > t = gen(s[i], s[j]);
			if (t.size() == (1 << total) - 1 && t.back() != string(total, '1')) {
				cout << "Length: " << total << ", " << s[i] << " " << s[j] << endl;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	//precalc();
	//return 0;

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
		} else {
			readData();
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
