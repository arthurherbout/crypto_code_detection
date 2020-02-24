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

const int NMAX = 16;

int N, M, R;
int A[NMAX][NMAX], W[NMAX][NMAX];
bool V[NMAX][NMAX];

bool check(int x, int y) {
	int i, j, r = 0;

	for (i = -1; i <= 1; ++i)
		for (j = -1; j <= 1; ++j) {
			if (x+i < 0 || x+i >= N || y+j < 0 || y+j >= M) continue;
			r += V[x+i][y+j];
		}
	
	return r == A[x][y];
}

void checksol(void) {
	int i, j, di, dj, cnt, x, y;

	for (i = 0; i < N; ++i)
		for (j = 0; j < M; ++j) {
			W[i][j] = 0;
			for (di = -1; di <= 1; ++di)
				for (dj = -1; dj <= 1; ++dj) {
					x = i + di; y = j + dj;
					if (x < 0 || x >= N || y < 0 || y >= M) continue;
					W[i][j] += V[x][y];
				}
			if (W[i][j] != A[i][j]) return;
		}
	
	cnt = 0;
	for (i = 0; i < M; ++i)
		cnt += V[N / 2][i];

	R = max(R, cnt);
}

void back(int x, int y) {
	if (y == M) ++x, y = 0;
	if (x == N) {
		checksol();	
		return;
	}

	V[x][y] = 0;
	if (x >= 1 && y >= 1) {
		if (check(x-1, y-1))
			back(x, y+1);
	} else back(x, y+1);

	V[x][y] = 1;
	if (x >= 1 && y >= 1) {
		if (check(x-1, y-1))
			back(x, y+1);
	} else back(x, y+1);
}

int main(void) {
	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN, i, j;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d %d", &N, &M);

		for (i = 0; i < N; ++i)
			for (j = 0; j < M; ++j)
				scanf(" %d", A[i] + j);

		R = 0;
		back(0, 0);
		
		printf("Case #%d: %d\n", cn, R);
	}

	return 0;

}

