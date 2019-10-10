#include <iostream>
#include <string.h>
#include <cstring>
#include <string>
#include <algorithm>
#include <math.h>
#include <vector>

using namespace std;

int n, m;
int u[3000], v[3000];

vector < vector < int > > cur;

void add(int u, int v)
{
	for (int i = 0; i < cur.size(); i ++)
	{
		bool have_u = false, have_v = false;
		for (int j = 0; j < cur[i].size(); j ++)
		{
			if (cur[i][j] == u) have_u = true;
			if (cur[i][j] == v) have_v = true;
		}
		if (have_u && have_v)
		{
			vector < int > n1, n2;
			n1.clear(), n2.clear();
			int pu, pv;
			for (int j = 0; j < cur[i].size(); j ++)
			{
				if (cur[i][j] == u) pu = j;
				if (cur[i][j] == v) pv = j;
			}
			if (pu > pv) swap(pu, pv);
			for (int j = 0; j <= pu; j ++)
				n1.push_back(cur[i][j]);
			for (int j = pv; j < cur[i].size(); j ++)
				n1.push_back(cur[i][j]);

			for (int j = pu; j <= pv; j ++)
				n2.push_back(cur[i][j]);
			cur[i] = n1;
			cur.push_back(n2);
			break;
		}
	}
}

int mx;
int used[20];
int best = 0;

int a[20];
int b[20];

void rec(int have, int colors)
{
	if (colors > mx) return ;
	if (have == n)
	{
		if (colors <= best) return ;
		bool nice = true;
		for (int i = 0; i < cur.size(); i ++)
		{
			bool good = true;
			for (int j = 1; j <= colors; j ++)
			{
				bool ch = false;
				for (int k = 0; k < cur[i].size(); k ++)
					if (b[cur[i][k]] == j) {ch = true; break;}
				if (!ch) {good = false; break;}
			}
			if (!good)
			{
				nice = false;
				break;
			}
		}
		if (nice)
		{
			best = colors;
			for (int i = 1; i <= n; i ++)
				a[i] = b[i];
		}
		return ;
	}
	for (int i = 1; i <= colors; i ++)
		b[have + 1] = i, rec(have + 1, colors);
	if (colors + 1 <= mx)
		b[have + 1] = colors + 1, rec(have + 1, colors + 1);
}


void solve(int test)
{
	scanf("%d%d", &n, &m);
	for (int i = 1; i <= m; i ++)
		scanf("%d", &u[i]);
	for (int i = 1; i <= m; i ++)
		scanf("%d", &v[i]);

	cur.clear();
	vector < int > start;
	for (int i = 1; i <= n; i ++)
		start.push_back(i);
	cur.push_back(start);
	for (int i = 1; i <= m; i ++)
		add(u[i], v[i]);

	mx = 0;
	for (int i = 0; i < cur.size(); i ++)
		mx = max(mx, (int)cur[i].size());
	for (int i = 1; i <= mx; i ++)
		used[i] = 0;
	best = 0;
	rec(0, 0);

	printf("Case #%d: %d\n", test, best);
	for (int i = 1; i < n; i ++)
		printf("%d ", a[i]);
	printf("%d\n", a[n]);
}

int main()
{
	freopen("input.txt","r",stdin);
	freopen("output.txt","w",stdout);
	int test;
	scanf("%d\n", &test);
	for (int i = 1; i <= test; i ++)
		solve(i);
	return 0;
}