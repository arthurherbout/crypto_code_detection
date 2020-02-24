#include<cmath>
#include<vector>
#include<cstdio>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

const int N = 1005;

int n, y[N];

double f;

char map[N];

int sign(double x, double eps = 1e-9) {
	return x < -eps ? -1 : x > eps;
}

int main() {
	int t;
	scanf("%d", &t);
	while (t--) {
		scanf("%d%lf%s", &n, &f, map);
		for (int i = 0; i < n; ++i) {
			y[i + 1] = y[i] + map[i] - '0';
		}
		double delta = 1e10;
		int ans = 0;
		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j <= n; ++j) {
				double t = (double)(y[j] - y[i]) / (j - i);
				if (sign(fabs(t - f) - delta) < 0) {
					ans = i;
					delta = fabs(t - f);
				}
			}
		}
		static int id = 0;
		printf("Case #%d: %d\n", ++id, ans);
	}
	return 0;
}
