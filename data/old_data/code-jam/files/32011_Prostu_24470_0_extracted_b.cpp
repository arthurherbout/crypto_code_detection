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

const int NMAX = 128;
bool V[NMAX][NMAX];

int main(void) {
	freopen("small.in", "rt", stdin);
	freopen("test.out", "wt", stdout);

	int cn, CN;
	int W, H, DX[2], DY[2], X, Y;
	int x, y, x1, y1, i, cnt;
	queue <PII> Q;

	scanf(" %d", &CN);

	for (cn = 1; cn <= CN; ++cn) {

		scanf(" %d %d", &W, &H);
		scanf(" %d %d", &DX[0], &DY[0]);
		scanf(" %d %d", &DX[1], &DY[1]);
		scanf(" %d %d", &X, &Y);

		memset(V, 0x00, sizeof(V));
		V[X][Y] = true; cnt = 0;
		for (Q.push(MP(X, Y)); !Q.empty(); Q.pop()) {
			x = Q.front().x;
			y = Q.front().y;
			++cnt;

			for (i = 0; i < 2; ++i) {
				x1 = x + DX[i]; y1 = y + DY[i];
				if (x1 >= 0 && x1 < W && y1 >= 0 && y1 < H && V[x1][y1] == false)
					Q.push(MP(x1, y1)), V[x1][y1] = true;
			}
		}

		
		printf("Case #%d: %d\n", cn, cnt);
	}

	return 0;

}

