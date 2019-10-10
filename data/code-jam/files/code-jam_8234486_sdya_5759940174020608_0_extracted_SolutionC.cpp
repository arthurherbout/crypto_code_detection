#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cstring>
#include <string>
#include <cmath>
#include <ctime>
#include <unordered_map>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

vector < string > parse(char s[]) {
	int n = strlen(s);
	vector < string > res;
	string buf = "";
	for (int i = 0; i <= n; ++i) {
		if (i == n || s[i] == ' ') {
			res.push_back(buf);
			buf = "";
		} else {
			buf += s[i];
		}
	}
	return res;
}

long long getHash(const string &s) {
	long long res = 0;
	for (int i = 0; i < s.size(); ++i) {
		res = res * 137LL + s[i];
	}
	return res;
}

const int maxL = 110000;
vector < vector < long long > > s;
int n;
char c[maxL];
unordered_map <long long, int> a, b;
int common = 0;

void addToA(long long value) {
	++a[value];
	if (a[value] == 1 && b[value] > 0) {
		++common;
	}
}

void addToB(long long value) {
	++b[value];
	if (b[value] == 1 && a[value] > 0) {
		++common;
	}
}

void removeFromA(long long value) {
	--a[value];
	if (a[value] == 0 && b[value] > 0) {
		--common;
	}
}

void removeFromB(long long value) {
	--b[value];
	if (b[value] == 0 && a[value] > 0) {
		--common;
	}
}

void rec(int m, int &res) {
	if (m == n) {
		res = min(res, common);
		return;
	}
	if (common >= res) {
		return;
	}

	for (int i = 0; i < s[m].size(); ++i) {
		addToA(s[m][i]);
	}
	rec(m + 1, res);
	for (int i = 0; i < s[m].size(); ++i) {
		removeFromA(s[m][i]);
	}

	for (int i = 0; i < s[m].size(); ++i) {
		addToB(s[m][i]);
	}
	rec(m + 1, res);
	for (int i = 0; i < s[m].size(); ++i) {
		removeFromB(s[m][i]);
	}
}

void solve(int test) {
	scanf("%d\n", &n);
	s.clear();
	for (int i = 0; i < n; ++i) {
		memset(c, 0, sizeof(c));
		gets(c);
		vector < string > cur = parse(c);
		vector < long long > a;
		for (int j = 0; j < cur.size(); ++j) {
			a.push_back(getHash(cur[j]));
		}
		s.push_back(a);
	}

	int res = 1000000000;
	a.clear();
	b.clear();
	common = 0;
	for (int j = 0; j < s[0].size(); ++j) {
		addToA(s[0][j]);
	}
	for (int j = 0; j < s[1].size(); ++j) {
		addToB(s[1][j]);
	}
	rec(2, res);
	printf("Case #%d: %d\n", test, res);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << (i + 1) << endl;
	}
	cerr << "Time: " << clock() << endl;
	return 0;
}