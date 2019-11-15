#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 10000;
long long n, a[maxN], res[maxN];
int no_solution;

void rec(long long l, long long r, long long A, long long B, long long C) {
	if (no_solution) {
		return ;
	}
	vector < long long > p;
	int current = l;
	while (current < r) {
		p.push_back(current);
		current = a[current];
	}

	if (current > r) {
		no_solution = true;
		return ;
	}

	int m = p.size();
	long long last = r;
	for (long long i = m - 1; i >= 0 && !no_solution; --i) {
		long long current = p[i];
		res[current] = (A * i + B - 1LL) / C;
		A = res[last] - res[current];
		B = last * res[current] - current * res[last];
		C = last - current;

		if (current + 1 < last) {
			rec(current + 1, last, A, B, C);
		}
		last = current;
	}
}

void solve(int test) {
	scanf("%d", &n);
	for (int i = 1; i < n; ++i)
		cin >> a[i];
	res[n] = 1000000000LL;
	no_solution = false;
	rec(1, n, 0, res[n], 1);

	if (no_solution) {
		printf("Case #%d: Impossible\n", test);
	} else {
		long long mn = 1000000000LL;
		for (int i = 1; i <= n; ++i) {
			mn = min(mn, res[i]);
		}
		printf("Case #%d:", test);
		for (int i = 1; i <= n; ++i) {
			cout << " " << res[i] - mn;
		}
		printf("\n");
	}
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << i << endl;
	}
	return 0;
}