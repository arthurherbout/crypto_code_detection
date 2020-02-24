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

int main(void) {
	int V[4][4];
	int X, Y, N, A, B, C, D, x0, y0, M;
	int i, j, CT, ct, u, v;
	vector <PII> P;
	llong R;

	freopen("large.in", "rt", stdin);
	freopen("large.out", "wt", stdout);

	scanf("%d", &CT);

	for (ct = 1; ct <= CT; ++ct) {
		scanf(" %d %d %d %d %d %d %d %d", &N, &A, &B, &C, &D, &x0, &y0, &M);
		R = 0;

		P.clear();

		X = x0; Y = y0;
		P.PB( MP(X, Y) );

		for (i = 1; i < N; ++i) {
			X = ((llong)A * X + B) % M;
			Y = ((llong)C * Y + D) % M;
			P.PB( MP(X, Y) );
//			printf("%d %d\n", X, Y);
		}

		for (i = 0; i < N; ++i) {
			memset(V, 0x00, sizeof(V));

			for (j = i+1; j < N; ++j)
				V[ P[j].x % 3 ][ P[j].y % 3 ]++;

			for (j = i+1; j < N; ++j) {
				V[ P[j].x % 3 ][ P[j].y % 3 ]--;
				u = (3 - (P[i].x + P[j].x) % 3) % 3;
				v = (3 - (P[i].y + P[j].y) % 3) % 3;
//				if (V[u][v]) printf("for %d %d got %d\n", i, j, V[u][v]);
//				printf("%d %d %d %d got %d\n", i, j, u, v, V[u][v]);

				R += V[u][v];
			}
		}

		printf("Case #%d: %lld\n", ct, R);
	}

	return 0;
}


