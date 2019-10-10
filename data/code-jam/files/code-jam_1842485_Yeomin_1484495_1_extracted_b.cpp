#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>

using namespace std;

#define X first
#define Y second
typedef pair<int, int> pii;
typedef pair<int, int> pdd;

int t, cases;
int n, w, l;
int v[1000];
pii p[1000];
pdd ret[1000];

double lng2(int dx, int dy)
{
	return dx * dx + dy * dy;
}

void solve()
{
	int x = 0, y = 0;
	int mx = 0;
	for (int i = n - 1; i >= 0; --i)
	{
		if (y != 0)
			y += p[i].X;
		if (y > l)
		{
			x += mx;
			if (x != 0)
				x += p[i].X + 1;
			y = 0;
			mx = 0;
		}
		if (x > w)
			assert(false);

		mx = max<int>(mx, p[i].X);
		ret[p[i].Y].X = x;
		ret[p[i].Y].Y = y;

		y += p[i].X + 1;
		if (y > l)
		{
			x += mx;
			if (i != 0)
				x += p[i - 1].X + 1;
			y = 0;
			mx = 0;
		}
		if (x > w)
			assert(false);
	}
}

int main()
{
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d%d%d", &n, &w, &l);
		for (int i = 0; i < n; ++i)
		{
			p[i].Y = i;
			scanf("%d", &p[i].X);
			v[i] = p[i].X;
		}

		sort(p, p + n);
		solve();

		fprintf(stderr, "%d\n", cases + 1);
		printf("Case #%d:", ++cases);
		for (int i = 0; i < n; ++i)
		{
			printf(" %d %d", ret[i].X, ret[i].Y);
		}
		printf("\n");
	}
}