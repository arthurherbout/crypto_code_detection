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

int m[5025], w[5025], ans[5035][25], P;

int solve_rec (int i, int j, int skip, int l, int r)
{
	if (l == r) {
		ans[i][skip] = (skip > m[r]) ? INT_MAX : 0;
		return m[r];
	}
	if (ans[i][skip] >= 0)
		return ans[i][skip];
	// skip
	int rl = solve_rec (i*2, j*2+1, skip+1, l, (l + r) >> 1);
	int rr = solve_rec (i*2+1, j*2, skip+1, ((l + r) >> 1) + 1, r);
	int mn = min (rl, rr);
	if (skip > mn) {
		ans[i][skip] = INT_MAX;
		return mn;
	}

	long long v1 = ((long long) ans[i*2][skip+1]) + ans[i*2+1][skip+1];
	if (v1 > INT_MAX) v1 = INT_MAX;
	// don't skip
	solve_rec (i*2, j*2+1, skip, l, (l+r) >> 1);
	solve_rec (i*2+1, j*2, skip, ((l+r)>>1)+1, r);
	long long v2 = ((long long)ans[i*2][skip]) + ans[i*2+1][skip] + w[(1 << P) - j];
	if (v2 > INT_MAX) v2 = INT_MAX;
	//assert (v2 < INT_MAX);

	ans[i][skip] = min(v1, v2);
	//printf ("%d %d %d = %d %Ld+%Ld\n", l, r, skip, ans[i][skip], v1, v2);
	return mn;
}

int solve ()
{
	int p;
	scanf ("%d", &p); P = p;
	for (int i = 1; i <= (1 << p); i++)
		scanf ("%d", &m[i]);
	for (int i = 1; i < (1 << p); i++)
		scanf ("%d", &w[i]);
	for (int i = 0; i < 5035; i++)
		for (int j = 0; j < 25; j++)
			ans[i][j] = -1;
	solve_rec (1, 1, 0, 1, 1 << p);
	//assert (ans[1][0] != INT_MAX);
	return ans[1][0];
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
