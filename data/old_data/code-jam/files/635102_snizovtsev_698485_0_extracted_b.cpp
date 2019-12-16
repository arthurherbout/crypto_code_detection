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

int m[1025], ans;

int solve_rec (int h, int l, int r)
{
	if (l == r)
		return m[r];
	int rl = solve_rec (h-1, l, (l + r) >> 1);
	int rr = solve_rec (h-1, ((l + r) >> 1) + 1, r);
	if (rl < h || rr < h)
		ans++;
	return min (rl, rr);
}

int solve ()
{
	int p;
	scanf ("%d", &p);
	for (int i = 1; i <= (1 << p); i++)
		scanf ("%d", &m[i]);
	for (int i = 1; i < (1 << p); i++) {
		int a;
		scanf ("%d", &a);
		assert (a == 1);
	}
	ans = 0;
	solve_rec (p, 1, 1 << p);
	return ans;
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
