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

const int NMAX = 128;
const int MOD = 10007;

bool V[NMAX][NMAX];
int D[NMAX][NMAX];

int main(void) {
	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;
	int H, W, R;
	int i, j, r, c;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf (" %d %d %d", &H, &W, &R);

		memset(V, 0x00, sizeof(V));
		for (i = 0; i < R; ++i) {
			scanf(" %d %d", &r, &c);
			V[r][c] = true;
		}

		memset(D, 0x00, sizeof(D));
		D[1][1] = 1;
		for (i = 1; i <= H; ++i)
			for (j = 1; j <= W; ++j) {
				if (V[i][j]) continue;
				if (i > 2 && j > 1)
					D[i][j] += D[i-2][j-1];
				if (i > 1 && j > 2)
					D[i][j] += D[i-1][j-2];
				if (D[i][j] >= MOD) D[i][j] -= MOD;
			}
		
		printf("Case #%d: %d\n", cn, D[H][W]);
	}

	return 0;
}


