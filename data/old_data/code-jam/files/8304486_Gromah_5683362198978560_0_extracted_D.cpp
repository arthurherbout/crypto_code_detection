#include <queue>
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long LL;
#define N 200 + 5
#define Mod 1000000007

const int Fx[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
int Case, res, n, m, q, d;
LL Map[N][N];

struct Point
{
	int x, y;
	Point (int _x = 0, int _y = 0) {x = _x, y = _y;}
	bool operator < (const Point a) const
	{
		return Map[x][y] > Map[a.x][a.y];
	}
};

priority_queue <Point> Q;

inline int Inc(int u, int v)
{
	return u + v - (u + v >= Mod ? Mod : 0);
}

inline LL Abs(LL x)
{
	return x > 0 ? x : -x;
}

int main()
{
	scanf("%d", &Case);
	for (int Test = 1; Test <= Case; Test ++)
	{
		scanf("%d%d%d%d", &n, &m, &q, &d);
		for (int i = 1; i <= n; i ++)
			for (int j = 1; j <= m; j ++)
				Map[i][j] = -1;
		while (!Q.empty()) Q.pop();
		for (int i = 1, u, v, w; i <= q; i ++)
		{
			scanf("%d%d%d", &u, &v, &w);
			Map[u][v] = w;
			Q.push(Point(u, v));
		}
		for (int T = 1; T <= n * m; T ++)
		{
			Point t = Q.top();
			Q.pop();
			int x = t.x, y = t.y;
			for (int k = 0; k < 4; k ++)
			{
				int tx = x + Fx[k][0], ty = y + Fx[k][1];
				if (tx && ty && tx <= n && ty <= m && !~Map[tx][ty])
				{
					Map[tx][ty] = Map[x][y] + d;
					Q.push(Point(tx, ty));
				}
			}
		}
		bool ok = 1;
		for (int i = 1; ok && i <= n; i ++)
			for (int j = 1; ok && j <= m; j ++)
				for (int k = 0; ok && k < 2; k ++)
				{
					int tx = i + Fx[k][0], ty = j + Fx[k][1];
					if (tx <= n && ty <= m && Abs(Map[i][j] - Map[tx][ty]) > d) ok = 0;
				}
		if (ok)
		{
			res = 0;
			for (int i = 1; i <= n; i ++)
				for (int j = 1; j <= m; j ++)
					res = Inc(res, Map[i][j] % Mod);
		}
		printf("Case #%d: ", Test);
		if (!ok) puts("IMPOSSIBLE");
		else printf("%d\n", res);
	}
	return 0;
}
