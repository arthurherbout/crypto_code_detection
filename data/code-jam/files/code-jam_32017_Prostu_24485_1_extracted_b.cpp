#include <vector>
#include <queue>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>

using namespace std;

#define FORI(it, x) for (typeof((x).begin()) it = (x).begin(); it != (x).end(); ++it)
#define ALL(x) (x).begin(), (x).end()
#define PB push_back
#define MP make_pair
#define x first
#define y second

typedef long long llong;
typedef vector <string> VS;
typedef vector <int> VI;
typedef pair <int, int> PII;

const int INF = 0x3f3f3f3f;

const int NMAX = 1 << 20;

int T[NMAX], W[NMAX], U[NMAX];
bool prim[NMAX];

int parinte(int k) {
	if (T[k] != k)
		T[k] = parinte(T[k]);
	return T[k];
}

void unite(int a, int b) {
	a = parinte(a);
	b = parinte(b);

	if (a == b) return;

	if (W[a] > W[b])
		T[b] = a;
	else
		T[a] = b;
	
	if (W[a] == W[b]) ++W[b];
}

int main(void) {
	llong A, B, C, D, P;
	llong l, j, c, p, R, i;

	freopen("large.in", "rt", stdin);
	freopen("test.out", "wt", stdout);	

	scanf(" %lld", &C);

	for (i = 4; i < NMAX; i += 2) prim[i] = true;
	for (i = 3; i < NMAX; i += 2) {
		if (prim[i]) continue;

		for (j = i+i; j < NMAX; j += i)
			prim[j] = true;
	}

	for (c = 1; c <= C; ++c) {

		scanf(" %lld %lld %lld", &A, &B, &P);
		R = 0;

		D = B - A + 1;

		for (i = 0; i < D; ++i)
			T[i] = i, W[i] = 0;

		for (i = P; i < D; ++i) {
			if (prim[i]) continue;
			l = A;
			if (A % i) l += i - A % i;
			for (j = l + i; j <= B; l = j, j += i)
//				printf("unite %lld %lld\n", l, j),
				unite(l - A, j - A);
		}

		for (i = 0; i < D; ++i) {
			p = parinte(i);

			if (U[p] != c) ++R, U[p] = c;
		}

		printf("Case #%lld: %lld\n", c, R);

	}

	return 0;
}


