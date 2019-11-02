#include <cstdio>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;

#define X first
#define Y second

int t, cases;
int n, m;
char table[60][60];
pair<int, int> pos[10];
int u;
vector<pair<int, int>> v, v2;
int used[60][60];

int qarr[3][2] = {{-1, 0}, {0, 1}, {0, -1}};

set<vector<pair<int, int>>> st;

bool solve2(vector<pair<int, int>>& prevV, int endPos);

bool tryDown(vector<pair<int, int>>& prevV, int endPos)
{
	bool s = true, changed = false;
	vector<pair<int, int>> cv = prevV;
	for (int i = 0; i < cv.size(); ++i)
	{
		if (table[cv[i].X + 1][cv[i].Y] != '#')
		{
			cv[i].X++;
			changed = true;
		}
		if (cv[i].X >= n || used[cv[i].X][cv[i].Y] == 0)
			return false;
		if (cv[i].X != pos[endPos].X || cv[i].Y != pos[endPos].Y)
			s = false;
	}
	if (st.find(cv) != st.end())
		return false;
	st.insert(cv);
	if (!changed) return false;
	if (s) return true;

	return solve2(cv, endPos);
}

bool tryLeft(vector<pair<int, int>>& prevV, int endPos)
{
	vector<pair<int, int>> cv = prevV;
	bool s = true, changed = false;
	for (int i = 0; i < cv.size(); ++i)
	{
		if (table[cv[i].X][cv[i].Y - 1] != '#')
		{
			changed = true;
			cv[i].Y--;
		}
		if (cv[i].Y < 0 || used[cv[i].X][cv[i].Y] == 0)
		{
			return false;
		}
		if (cv[i].X != pos[endPos].X || cv[i].Y != pos[endPos].Y)
			s = false;
	}
	if (st.find(cv) != st.end())
		return false;
	st.insert(cv);
	if (!changed) return false;
	if (s) return true;

	return solve2(cv, endPos);
}

bool tryRight(vector<pair<int, int>>& prevV, int endPos)
{
	vector<pair<int, int>> cv = prevV;
	bool s = true, changed = false;
	for (int i = 0; i < cv.size(); ++i)
	{
		if (table[cv[i].X][cv[i].Y + 1] != '#')
		{
			changed = true;
			cv[i].Y++;
		}
		if (cv[i].Y < 0 || used[cv[i].X][cv[i].Y] == 0)
		{
			return false;
		}
		if (cv[i].X != pos[endPos].X || cv[i].Y != pos[endPos].Y)
			s = false;
	}
	if (st.find(cv) != st.end())
		return false;
	st.insert(cv);
	if (!changed) return false;
	if (s) return true;

	return solve2(cv, endPos);
}

bool solve2(vector<pair<int, int>>& prevV, int endPos)
{
	if (tryDown(prevV, endPos))
		return true;
	if (tryLeft(prevV, endPos))
		return true;
	if (tryRight(prevV, endPos))
		return true;

	return false;
}

void solve()
{
	for (int i = 0; i < u; ++i)
	{
		printf("%d: ", i);
		v.clear();
		
		queue<pair<int, int>> q;
		pair<int, int> c, d;
		c = pos[i];
		q.push(c);
		fill(used[0], used[60], 0);
		v.push_back(c);
		used[c.X][c.Y] = 1;
		while (!q.empty())
		{
			c = q.front(); q.pop();
			for (int i = 0; i < 3; ++i)
			{
				d.X = c.X + qarr[i][0]; d.Y = c.Y + qarr[i][1];
				if (d.X < 0 || d.X >= n || d.Y < 0 || d.Y >= m || table[d.X][d.Y] == '#')
					continue;
				if (used[d.X][d.Y]) continue;
				used[d.X][d.Y] = 1;
				v.push_back(d);
				q.push(d);
			}
		}
		printf("%d ", v.size());
		st.clear();
		st.insert(v);

		if (v.size() == 1 || solve2(v, i))
			printf("Lucky");
		else
			printf("Unlucky");
		printf("\n");
	}
}

int main()
{
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d%d", &n, &m);
		u = 0;
		for (int i = 0; i < n; ++i)
		{
			scanf("%s", table[i]);
			for (int j = 0; j < m; ++j)
			{
				if (table[i][j] >= '0' && table[i][j] <= '9')
				{
					int p = table[i][j] - '0';
					pos[p].X = i; pos[p].Y = j;
					u = max(u, p + 1);
				}
			}
		}

		printf("Case #%d:\n", ++cases);
		solve();
	}
}