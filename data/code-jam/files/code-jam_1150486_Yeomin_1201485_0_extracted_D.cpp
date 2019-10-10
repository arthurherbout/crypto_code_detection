#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int t, cases, n, m;
int level[36];
vector<int> v[36];
int res = 0;
int seld[36], seld2[36];

void solve(int cur, int lv)
{
	if (lv == level[1])
	{
		if (cur == 1)
		{
			int intrude = 0;
			for (int i = 0; i < lv; ++i)
			{
				int& c = seld2[i];
				for (int j =0; j < v[c].size(); ++j)
				{
					int& d = v[c][j];
					if (seld[d] == 0)
					{
						seld[d] = 2;
						++intrude;
					}
				}
			}

			res = max(res, intrude);
			for (int i = 0; i < n; ++i)
				if (seld[i] == 2) seld[i] = 0;
		}
		return;
	}

	seld[cur] = 1;
	seld2[lv] = cur;
	for (int i = 0; i < v[cur].size(); ++i)
	{
		int& d = v[cur][i];
		if (level[d] == lv + 1)
			solve(d, lv + 1);
	}
	seld[cur] = 0;
}

int main()
{
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d%d", &n, &m);
		for (int i = 0; i < n; ++i) v[i].clear();
		for (int i = 0; i < m; ++i)
		{
			int x, y;
			scanf("%d,%d", &x, &y);
			v[x].push_back(y);
			v[y].push_back(x);
		}
		queue<int> q;
		q.push(0);
		fill(level, level + n, -1);
		level[0] = 0;
		while (!q.empty())
		{
			int c = q.front(); q.pop();
			for (int i = 0; i < v[c].size(); ++i)
			{
				int& d = v[c][i];
				if (level[d] == -1)
				{
					level[d] = level[c] + 1;
					q.push(d);
				}
			}
		}

		fill(seld, seld + n, 0);
		fill(seld2, seld2 + n, 0);
		res = 0;
		solve(0, 0);

		printf("Case #%d: %d %d\n", ++cases, level[1] - 1, res);
	}
}