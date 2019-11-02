#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <bitset>
#include <set>
#include <string>
#include <map>
#include <cmath>
#include <queue>
#include <deque>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const double eps = 1e-9;

void solve(int test) {
	int n;
	cin >> n;
	vector < double > a(n);
	vector < double > x(n);
	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}

	double left_bound = 0.0, right_bound = 100.0;
	for (int iters = 0; iters < 100; ++iters) {
		double middle = (left_bound + right_bound) / 2.0;

		bool have_solution = false;

		for (int i = 0; i < n && !have_solution; ++i) {
			for (int j = 0; j < n && !have_solution; ++j) {
				if (i == j) {
					continue;
				}

				x[i] = a[i] - middle;
				x[j] = a[j] + middle;

				double d = (x[j] - x[i]) / (double)(j - i);
				for (int k = 0; k < n; ++k) {
					x[k] = x[i] + d * (double)(k - i);
				}

				bool nice = true;
				for (int k = 0; k < n; ++k) {
					if (x[k] >= a[k] - middle - eps && x[k] <= a[k] + middle + eps) {
						continue;
					}
					nice = false;
				}
				if (nice) {
					have_solution = true;
				}
			}
		}

		if (have_solution) {
			right_bound = middle;
		} else {
			left_bound = middle;
		}
	}

	printf("Case #%d: %.10lf\n", test, (left_bound + right_bound) / 2.0);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int test = 0; test < tests; ++test) {
		solve(test + 1);
		cerr << test << endl;
	}

	cerr << "Time: " << clock() << endl;
	return 0;
}
