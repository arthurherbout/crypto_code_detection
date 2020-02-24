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

int N, M, A;
int x2, y2, x3, y3;

bool found(void) {
	int i, j, k, t;

	for (i = 0; i <= N; ++i)
		for (j = 0; j <= M; ++j)
			for (k = 0; k <= N; ++k)
				for (t = 0; t <= M; ++t)
					if ( abs(i * t - j * k) == A ) {
						x2 = i; y2 = j;
						x3 = k; y3 = t;
						return true;
					}

	return false;
}

int main(void) {
	freopen("small5.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d %d %d", &N, &M, &A);

		printf("Case #%d: ", cn);
		if (found())
			printf("0 0 %d %d %d %d\n", x2, y2, x3, y3);
		else
			printf("IMPOSSIBLE\n");
		
	}

	return 0;
}


