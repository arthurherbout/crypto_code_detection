#include <cstdio>
#include <cmath>
#include <set>
#include <map>

using namespace std;

char cmap[30][31];
int ds[4];	// top, right, bottom, left

int h, w, d;
set<pair<int, int> > ps;

int gcd(int a, int b)
{
	if (a%b == 0)
		return b;

	return gcd(b, a%b);
}

void dist(int y, int x, int& dy, int& dx)
{
	int ydir = 2;
	if (y<0)
	{
		ydir = 0;
		y = -y;
	}

	int xdir = 1;
	if (x<0)
	{
		xdir = 3;
		x = -x;
	}

	dy = y/2*2*(h-2) + y%2*ds[ydir];
	if (ydir == 0)
		dy = -dy;

	dx = x/2*2*(w-2) + x%2*ds[xdir];
	if (xdir == 3)
		dx = -dx;
}

void put2(int y, int x)
{
	int dy, dx;
	dist(y, x, dy, dx);
	if (dy*dy + dx*dx <= d)
	{
		if (dy == 0)
			ps.insert(make_pair(0, dx/abs(dx)));
		else if (dx == 0)
			ps.insert(make_pair(dy/abs(dy), 0));
		else
		{
			int g = gcd(abs(dy), abs(dx));
			ps.insert(make_pair(dy/g, dx/g));
		}
	}
}

int main()
{
	int t;
	scanf("%d", &t);
	for (int c=1; c<=t; ++c)
	{		
		scanf("%d %d %d", &h, &w, &d);
		d = d*d;
		gets(cmap[0]);
		for (int i=0; i<h; ++i)
		{
			gets(cmap[i]);
			for (int j=0; j<w; ++j)
			{
				if (cmap[i][j] == 'X')
				{
					ds[0] = (i<<1) - 1;
					ds[1] = ((w-1-j)<<1) - 1;
					ds[2] = ((h-1-i)<<1) - 1;
					ds[3] = (j<<1) - 1;
				}
			}
		}

		ps.clear();

		for (int i=1; i<=50; ++i)
		{
			// (-i,-i) ~ (-i, i)
			for (int j=-i; j<i; ++j)
				put2(-i, j);

			// (-i, i) ~ (i, i)
			for (int j=-i; j<i; ++j)
				put2(j, i);

			// (i, i) ~ (i, -i)
			for (int j=i; j>-i; --j)
				put2(i, j);

			// (i, -i) ~ (-i, -i)
			for (int j=i; j>-i; --j)
				put2(j, -i);
		}

		printf("Case #%d: %d\n", c, ps.size());
	}
	
	return 0;
}