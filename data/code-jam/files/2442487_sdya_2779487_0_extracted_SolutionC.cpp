#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <bitset>
#include <queue>
#include <cmath>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 2100;
int n, a[maxN], b[maxN];
int x[maxN];
bool used[maxN];

bool have_answer;
int d[maxN], e[maxN];

bool check(int length) {
	d[length] = 1;
	for (int j = 1; j < length; ++j) {
		if (x[length] > x[j]) {
			d[length] = max(d[length], 1 + d[j]);
		}
	}
	if (d[length] != a[length]) {
		return false;
	}

	if (length == n) {
		for (int i = length; i >= 1; --i) {
			e[i] = 1;
			for (int j = i + 1; j <= length; ++j) {
				if (x[i] > x[j]) {
					e[i] = max(e[i], 1 + e[j]);
				}
			}
			if (e[i] > b[i]) {
				return false;
			}
			if (n == length && e[i] != b[i]) {
				return false;
			}
		}
	}
	return true;
}

void rec(int have) {
	if (have == n) {
		have_answer = true;
		return;
	}

	int c = 0, d = 0;
	for (int i = 1; i < a[have + 1] + b[have + 1] - 1; ++i) {
		if (!used[i]) {
			++c;
		} else {
			++d;
		}
	}
	for (int i = a[have + 1] + b[have + 1] - 1; i <= n && !have_answer; ++i) {
		if (used[i]) {
			++d;
			continue;
		}
		++c;
		x[have + 1] = i;
		used[i] = true;
		if (d + 1 >= a[have + 1] && c >= b[have + 1] && check(have + 1)) {
			rec(have + 1);
		}
		used[i] = false;
	}
}

void solve(int test) {
	cin >> n;
	for (int i = 1; i <= n; ++i) {
		cin >> a[i];
	}
	for (int i = 1; i <= n; ++i) {
		cin >> b[i];
	}

	memset(used, 0, sizeof(used));
	have_answer = false;
	rec(0);

	cout << "Case #" << test << ":";
	for (int i = 1; i <= n; ++i) {
		cout << " " << x[i];
	}
	cout << endl;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d\n", &tests);
	for (int i = 1; i <= tests; ++i) {
		solve(i);
		cerr << i << endl;
	}

	return 0;
}