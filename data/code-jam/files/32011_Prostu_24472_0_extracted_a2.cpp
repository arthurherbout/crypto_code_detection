#include <cstdio>
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>

using namespace std;

#define PB push_back
#define MP make_pair
#define x first
#define y second
#define ALL(X) (X).begin(), (X).end()
#define FORI(p, X) for (__typeof((X).begin()) p = (X).begin(); p != (X).end(); ++p)

typedef pair <int, int> PII;
typedef long long lint;
typedef vector <int> VI;

const int NMAX = 5005;
const int MAX = 10000;

int N, A[NMAX], B[NMAX], C[NMAX];

int prop(int a, int b, int c) {
	int i, cnt = 0;

	for (i = 0; i < N; ++i)
		if (A[i] <= a && B[i] <= b && C[i] <= c)
			++cnt;

	return cnt;
}

int seek(int st, int dr, int f) {
	if (dr - st <= 10) {
		int i, R = 0;
		for (i = st; i <= dr; ++i) {
			R = max(R, prop(f, i, MAX - (f+i)));
		}
		return R;
	}

	int i, j;
	i = (2 * st + dr) / 3;
	j = (st + 2 * dr) / 3;

	if (prop(f, i, MAX - (f + i)) > prop(f, j, MAX - (f + j)))
		return seek(st, j, f);
	return seek(i, dr, f);
}

int main(void) {
	freopen("small2.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;
	int R, i, j;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d", &N);
		R = 0;

		for (i = 0; i < N; ++i)
			scanf(" %d %d %d", A + i, B + i, C + i);

		for (i = 0; i <= MAX; ++i)
			for (j = 0; i + j <= MAX; ++j)
				R = max(R, prop(i, j, MAX - (i + j)));
		
		printf("Case #%d: %d\n", cn, R);
	}

	return 0;

}

