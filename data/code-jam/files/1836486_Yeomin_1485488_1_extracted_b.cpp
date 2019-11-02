#include <iostream>
#include <algorithm>
#include <queue>

using namespace std;

int t, h, n, m, cases;
int C[100][100];
int F[100][100];
int v[100][100];

struct in_que
{
	int x, y;
	int time;

	bool operator < (const in_que& rhs) const
	{
		return time > rhs.time;
	}
};

int arr[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int solve()
{
	fill(v[0], v[100], -1);
	v[0][0] = 0.0;

	in_que c, d;
	priority_queue<in_que> q;
	c.x = c.y = 0;
	c.time = 0;
	q.push(c);
	
	while (!q.empty())
	{
		c = q.top(); q.pop();
		if (v[c.x][c.y] != c.time) continue;
		//fprintf(stderr, "c - x: %d, y: %d, time: %d\n", c.x, c.y, c.time);
		
		for (int i = 0; i < 4; ++i)
		{
			d.x = c.x + arr[i][0];
			d.y = c.y + arr[i][1];
			if (d.x < 0 || d.x >= n || d.y < 0 || d.y >= m) continue;

			int curC = C[c.x][c.y];
			int curF = max(h - c.time, F[c.x][c.y]);
			int curRF = F[c.x][c.y];
			int nexC = C[d.x][d.y];
			int nexF = max(h - c.time, F[d.x][d.y]);
			int nexRF = F[d.x][d.y];

			if (nexC - curRF < 50 || nexC - nexRF < 50 || curC - nexRF < 50) continue;
			if (nexC - curF >= 50 && nexC - nexF >= 50)
			{	// can go now!
				if (c.time == 0)
					d.time = 0;
				else
					d.time = c.time + ((h - c.time - 20 < curRF) ? 100 : 10);
			}
			else
			{	// waiting
				int waiting = max(curF - (nexC - 50), nexF - (nexC - 50));
				d.time = c.time + waiting + ((h - c.time - waiting - 20 < curRF) ? 100 : 10);
			}
			if (v[d.x][d.y] == -1 || v[d.x][d.y] > d.time)
			{
				v[d.x][d.y] = d.time;
				q.push(d);
			}
		}
	}

	return v[n - 1][m - 1];
}

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d%d%d", &h, &n, &m);
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				scanf("%d", &C[i][j]);
			}
		}
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				scanf("%d", &F[i][j]);
			}
		}
		printf("Case #%d: %lf\n", ++cases, solve() / 10.f);
	}
}