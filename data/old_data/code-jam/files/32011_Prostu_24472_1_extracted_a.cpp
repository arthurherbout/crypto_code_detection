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
bool V[NMAX];

int prop(int a, int b, int c) {
	int i, cnt = 0;

	for (i = 0; i < N; ++i)
		if (A[i] <= a && B[i] <= b && C[i] <= c)
			++cnt;

	return cnt;
}

int seek(int f) {
	int i, j, bi, ci, u;
	vector<PII> b, c;
	int R = 0, cnt;

	memset(V, 0x00, sizeof(V));

	for (i = 0; i < N; ++i)
		if (A[i] <= f)
			b.PB(MP(B[i], i)),
			c.PB(MP(C[i], i));

	sort(ALL(b));
	sort(ALL(c));

	bi = 0; ci = c.size() - 1; cnt = 0;
	for (i = 0; f + i <= MAX; ++i) {
		j = MAX - f - i;

		while (bi < (int)b.size() && b[bi].x <= i) {
			u = b[bi].y;
			if (C[u] <= j)
				V[u] = true, ++cnt;
			++bi;
		}
		while (ci >= 0 && c[ci].x > j) {
			u = c[ci].y;
			if (V[u] == true)
				V[u] = false, --cnt;
			--ci;
		}
		R = max(R, cnt);
	}

	return R;
}

int main(void) {
	freopen("large.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;
	int R, i;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d", &N);
		R = 0;

		for (i = 0; i < N; ++i)
			scanf(" %d %d %d", A + i, B + i, C + i);

		for (i = 0; i <= MAX; ++i) {
//			if (seek(i) > R) printf("%d %d\n", i, seek(i));
			R = max(R, seek(i));
		}
		
		printf("Case #%d: %d\n", cn, R);
	}

	return 0;

}

