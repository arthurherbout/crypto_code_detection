#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
#include <map>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <bitset>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 11000;
int n, k;
int c[maxN], a[maxN], b[maxN];

struct rat {
	long long a, b;
	rat() {
		a = 0;
		b = 1;
	}
	rat(long long a, long long b) : a(a), b(b) {
	}

	bool operator < (const rat &other) const {
		return a * other.b < other.a * b;
	}

	double value() const {
		return (double)(a) / (double)(b);
	}
};

vector < rat > have;

double calculate(const vector < int > &ind) {
	double A = 1.0, B = 1.0;
	for (int i = 0; i < k; ++i) {
		A = A * (1.0 - have[ind[i]].value()) + B * have[ind[i]].value();
		B *= have[ind[i]].value();
	}
	return 1.0 - A;
}

const int maxS = 1100000;
double A[maxS], rA[maxS];
double P[maxS], rP[maxS];

void gen() {
	printf("1\n10000\n");
	for (int i = 0; i < 10000; ++i) {
		int x = rand(), y = rand();
		if (x > y) {
			swap(x, y);
		}
		printf("%d/%d %d\n", x, y, 100);
	}
}

void solve(int test) {
	scanf("%d%d", &n, &k);
	have.clear();
	for (int i = 1; i <= n; ++i) {
		scanf("%d/%d%d", &a[i], &b[i], &c[i]);
		for (int j = 0; j < c[i]; ++j) {
			have.push_back(rat(a[i], b[i]));
		}
	}

	sort(have.begin(), have.end());
	reverse(have.begin(), have.end());

	vector < int > answer;
	answer.push_back(0);
	for (int i = 0; i < k; ++i) {
		answer.push_back(i);
	}
	A[0] = 1.0, P[0] = 1.0;
	rA[k + 1] = 1.0, rP[k + 1] = 1.0;
	for (int i = 1; i <= k; ++i) {
		double p = have[i - 1].value();
		A[i] = A[i - 1] * (1.0 - p) + P[i - 1] * p;
		P[i] = P[i - 1] * p;
	}

	for (int i = k; i >= 1; --i) {
		double p = have[i - 1].value();
		rA[i] = rA[i + 1] * p + rP[i + 1] * (1.0 - p);
		rP[i] = rP[i + 1] * (1.0 - p);
	}

	double res = A[k];
	const double eps = 1e-10;

	for (int i = k; i >= 1; --i) {
		int bound = have.size() - 1;
		if (i != k) {
			bound = min(bound, answer[i + 1] - 1);
		}

		int old = answer[i];
		answer[i] = bound;

		double v1 = A[i - 1] * rP[i + 1];
		double v2 = P[i - 1] * rA[i + 1];

		if (v1 > v2 - eps) {
			double p = have[bound].value();
			A[i] = A[i - 1] * (1.0 - p) + P[i - 1] * p;
			P[i] = P[i - 1] * p;

			rA[i] = rA[i + 1] * p + rP[i + 1] * (1.0 - p);
			rP[i] = rP[i + 1] * (1.0 - p);

			res = v1 * (1.0 - p) + v2 * p;
		} else {
			answer[i] = old;
		}
	}

	printf("Case #%d: %.10lf\n", test, 1.0 - res);
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	//gen();
	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << i << endl;
	}
	cerr << clock() << endl;
	return 0;
}