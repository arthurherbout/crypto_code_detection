#include <cstdio>
#include <iostream>
using namespace std;
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include <cstdlib>
#include <cmath>
#include <string>

void initialize() {
	freopen("a.in", "r", stdin);
	freopen("a.out", "w", stdout);
}

const int MOD = 10009;

int doit(const int c[26], const string &a, int L, int R) {
	int res = 0;
	int tmp = 1;
	for (int i = L; i <= R; ++i) {
		if (a[i] == '+') {
			res = res + tmp;
			if (res >= MOD) res %= MOD;
			tmp = 1;
		}
		else {
			tmp = (tmp * c[a[i] - 'a']);
			if (tmp >= MOD) tmp %= MOD;
		}
	}
	res += tmp;
	return res % MOD;
}

int n, k;
vector<string> a;
string polynom;
int c[26];
long long result[30];

int C[30][30];

int e[30];

void DFS(int depth, int from) {
//	result[depth] = (result[depth] + doit(c, polynom, 0, polynom.size() - 1)) % MOD;
	int koeff = doit(c, polynom, 0, polynom.size() - 1);
	int rst = depth;
	for (int i = 0; i < n; ++i) if (e[i] != 0) {
		koeff = (koeff * C[rst][e[i]]) % MOD;
		rst -= e[i];
	}
	result[depth] = (result[depth] + koeff) % MOD;
	if (depth == k) return;
	for (int next = from; next < n; ++next) {
		if (depth == 0)
			cerr << "   " << next << endl;
		for (int j = 0; j < a[next].size(); ++j)
			++c[a[next][j] - 'a'];
		++e[next];
		DFS(depth + 1, next);
		--e[next];
		for (int j = 0; j < a[next].size(); ++j)
			--c[a[next][j] - 'a'];
	}
}

void solve() {
	cin >> polynom >> k >> n;
	a.resize(n);
	for (int i = 0; i < n; ++i)
		cin >> a[i];
	for (int i = 0; i < 30; ++i)
		result[i] = 0;
	DFS(0, 0);
	for (int i = 1; i <= k; ++i)
		cout << result[i] << " ";
	cout << endl;
}

int main() {

	for (int i = 0; i < 30; ++i)
		for (int j = 0; j < 30; ++j) {
			if (j == 0)
				C[i][j] = 1;
			else
			if (j == i)
				C[i][j] = 1;
			else
				if (j > i)
					C[i][j] = 0;
				else
					C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
		}
	initialize();
	int t;
	cin >> t;
	for (int i = 0; i < t; ++i) {
		cout << "Case #" << i + 1 << ": ";
		cerr << i + 1 << endl;
		solve();
	}
	return 0;
}
