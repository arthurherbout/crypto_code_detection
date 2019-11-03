#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
	for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)

bool rect[101][101];
int solve ()
{
	memset (rect, false, sizeof (rect));
	int r, b = 0;
	scanf ("%d", &r);
	for (int i = 0; i < r; i++) {
		int x1, y1, x2, y2;
		scanf ("%d %d %d %d", &x1, &y1, &x2, &y2);
		for (int x = x1; x <= x2; x++) {
			for (int y = y1; y <= y2; y++) {
				b += rect[x][y] ? 0 : 1;
				rect[x][y] = true;
			}
		}
	}

	int t = 0;
	while (b > 0) {
		for (int x = 100; x >= 1; x--) {
			for (int y = 100; y >= 1; y--) {
				if (rect[x][y]) {
					if (!rect[x-1][y] && !rect[x][y-1]) {
						rect[x][y] = false;
						b--;
					}
				} else {
					if (rect[x-1][y] && rect[x][y-1]) {
						rect[x][y] = true;
						b++;
					}
				}
			}
		}
		t++;
	}
	return t;
}

int main ()
{
	int T;
	scanf ("%d", &T);
	for (int i = 1; i <= T; i++) {
		printf ("Case #%d: %d\n", i, solve ());
	}
	return 0;
}
