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

const int NMAX = 5005;

int main(void) {
	int T[NMAX], V[NMAX];
	int CT, K, n;
	int i, t, j, N, p, u;

	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	scanf(" %d", &CT);

	for (t = 1; t <= CT; ++t) {
		scanf(" %d %d", &K, &n);

		for (i = 1; i <= K; ++i)
			T[i] = i;

		p = 1; N = K;
		for (i = 1; i <= K; ++i) {
			for (j = 1; j < i; ++j) {
				++p;
				if (p > N) p = 1;
			}
			V[ T[p] ] = i;
			for (j = p; j < N; ++j)
				T[j] = T[j+1];
			--N;
			if (p > N) p = 1;
		}

		printf("Case #%d: ", t);

		for (i = 0; i < n; ++i) {
			scanf(" %d", &u);
			printf("%d ", V[u]);
		}
		printf("\n");
	}

	return 0;
}


