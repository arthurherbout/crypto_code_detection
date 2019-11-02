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
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const double eps = 1e-9;
const int maxN = 110;

int n;
double V, X;
double r[maxN], c[maxN];
double p[maxN];

double solve(double A1, double B1, double C1, double A2, double B2, double C2) {
	/*{
		double d = min(fabs(A1), min(fabs(B1), fabs(C1)));
		A1 /= d;
		B1 /= d;
		C1 /= d;
	}
	{
		double d = min(fabs(A2), min(fabs(B2), fabs(C2)));
		A2 /= d;
		B2 /= d;
		C2 /= d;
	}*/
	double D = (A1 * B2 - A2 * B1);
	double dx = C1 * B2 - B1 * C2;
	double dy = A1 * C2 - A2 * C1;

	if (fabs(D) < eps) {
		double t = C1 / (A1 + B1);
		double value = A2 * t + B2 * t;
		if (fabs(value - C2) < eps) {
			return t;
		}
	}

	if (D != 0.0) {
		double x = dx / D;
		double y = dy / D;
		double v1 = x * A1 + y * B1 - C1;
		v1 /= C1;
		double v2 = x * A2 + y * B2 - C2;
		v2 /= C2;
		if (x >= -eps && y >= -eps && fabs(v1) < eps && fabs(v2) < eps) {
			return max(x, y);
		}
	}
	return 1e100;
}

void solve(int test) {
	cin >> n >> V >> X;
	for (int i = 0; i < n; ++i) {
		cin >> r[i] >> c[i];
	}

	vector < double > a(n), b(n), d(n);
	for (int i = 0; i < n; ++i) {
		a[i] = r[i];
		b[i] = r[i] * c[i] / X;
		d[i] = a[i] - b[i];
	}

	double res = 1e100;
	double sum = 0.0;
	for (int i = 0; i < n; ++i) {
		if (fabs(c[i] - X) < 1e-9) {
			sum += r[i];
		}
	}
	if (sum > 1e-9) {
		res = min(res, V / sum);
	}

	if (test == 93) {
		int r = 0;
	}

	for (int i = 0; i < n; ++i) {
		for (int j = i + 1; j < n; ++j) {
			if (d[i] < -eps && d[j] > eps) {
				double x = d[j];
				double y = -d[i];

				double have = r[i] * x + r[j] * y;
				double coef = V / have;

				double left_bound = 0.0, right_bound = max(x, y) * coef;
				for (int k = 0; k < 100; ++k) {
					double middle = (left_bound + right_bound) / 2.0;
					
					double cf = middle / max(x, y);
					double nx = cf * x;
					double ny = cf * y;

					double value = r[i] * nx + r[j] * ny + middle * sum;
					if (value >= V) {
						right_bound = middle;
					} else {
						left_bound = middle;
					}
				}
				res = min(res, left_bound);

				continue;
			}
			if (d[i] > eps && d[j] < -eps) {
				double x = -d[j];
				double y = d[i];

				double have = r[i] * x + r[j] * y;
				double coef = V / have;
				double left_bound = 0.0, right_bound = max(x, y) * coef;
				for (int k = 0; k < 100; ++k) {
					double middle = (left_bound + right_bound) / 2.0;
					
					double cf = middle / max(x, y);
					double nx = cf * x;
					double ny = cf * y;

					double value = r[i] * nx + r[j] * ny + middle * sum;
					if (value >= V) {
						right_bound = middle;
					} else {
						left_bound = middle;
					}
				}
				res = min(res, left_bound);

				continue;
			}
		}
	}

	if (res > 1e50) {
		printf("Case #%d: IMPOSSIBLE\n", test);
		return;
	}
	printf("Case #%d: %.10lf\n", test, res);
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