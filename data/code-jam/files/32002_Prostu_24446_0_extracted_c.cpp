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

const int NMAX = 10;

int N, M, A[NMAX];
int D[NMAX][1 << NMAX];

bool copie(int u, int v) {
	return ((u << 1) & v) || ((u >> 1) & v);
	int i;

	for (i = 1; i + 1 < M; ++i)
		if ((u & (1 << i)) && ((v & (1 << (i - 1))) || (v & (1 << (i + 1))) ))
			return true;
	
	return false;
}

int main(void) {
	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;
	char ch;
	int i, j, k, cnt, R;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d %d\n", &N, &M);

		for (i = 0; i < N; ++i) {
			A[i] = 0;
			for (j = 0; j < M; ++j) {
				scanf("%c", &ch);

				if (ch == 'x')
					A[i] |= 1 << j;
			}
			scanf("\n");
		}

		memset(D, 0xd0, sizeof(D));
		for (i = 0; i < (1 << M); ++i)
			if ((i & A[0]) == 0 && !copie(i, i)) {
				D[0][i] = 0;
				for (j = 0; j < M; ++j)
					if (i & (1 << j))
						++D[0][i];
			}

		for (i = 1; i < N; ++i) {
			for (j = 0; j < (1 << M); ++j) {
				if (copie(j, j) || (A[i] & j)) continue;
				for (k = cnt = 0; k < M; ++k)
					if (j & (1 << k)) ++cnt;

				for (k = 0; k < (1 << M); ++k) {
					if (copie(j, k)) continue;
					D[i][j] = max(D[i][j], D[i-1][k] + cnt);
				}
			}
		}

		for (R = i = 0; i < (1 << M); ++i)
			R = max(R, D[N-1][i]);
		
		printf("Case #%d: %d\n", cn, R);
	}

	return 0;
}


