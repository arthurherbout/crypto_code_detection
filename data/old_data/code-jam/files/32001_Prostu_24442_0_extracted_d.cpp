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
const int NMAX = 1024;

int N;
char S[NMAX], T[NMAX];

int RLE() {
	int i, r = 1;
	char c = T[0];

	for (i = 1; T[i]; ++i)
		if (c != T[i])
			c = T[i], ++r;
	
	return r;
}

int run(void) {
	int P[8], mn = INF;
	int i, j;

	T[strlen(S)] = 0;

	for (i = 0; i < N; ++i) P[i] = i;

	do {

		for (i = 0; S[i]; i += N)
			for (j = 0; j < N; ++j)
				T[i + j] = S[i + P[j]];

		mn = min(mn, RLE());

	} while (next_permutation(P, P + N));

	return mn;
}

int main(void) {
	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d %s", &N, S);

		printf("Case #%d: %d\n", cn, run());
	}

	return 0;
}


