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

int n, k, b, t;
int x[50], v[50];

bool can_reach (int i)
{
	return (b - x[i]) <= (t * v[i]);
}

void solve ()
{
	scanf ("%d %d %d %d", &n, &k, &b, &t);
	for (int i = 0; i < n; i++)
		scanf ("%d", &x[i]);
	for (int i = 0; i < n; i++)
		scanf ("%d", &v[i]);

	int max_chicks = 0;
	for (int i = n-1; i >= 0; i--) {
		if (can_reach (i))
			max_chicks++;
	}
	if (max_chicks < k) {
		printf ("IMPOSSIBLE\n");
		return;
	}

	int swaps = 0, res = 0;
	for (int i = n-1; i >= 0 && k > 0; i--) {
		if (can_reach (i)) {
			k--;
			res += swaps;
		} else {
			swaps++;
		}
	}
	printf ("%d\n", res);
}

int main ()
{
	int T;
	scanf ("%d", &T);
	for (int i = 1; i <= T; i++) {
		printf ("Case #%d: ", i);
		solve ();
	}
	return 0;
}
