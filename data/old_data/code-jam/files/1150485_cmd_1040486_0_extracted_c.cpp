#if 1
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <sstream>
#include <stack>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <functional>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <cstdio>
#include <list>
#include <ctime>
using namespace std;

typedef long long LL;
typedef long double LD;
const LD eps = 1e-9;

typedef pair<int, int> pii;
#define mp make_pair
#define pb push_back
#define X first
#define Y second
#define iss istringstream
#define oss ostringstream
#define dbg(x) cerr << #x << " = " << x << endl;
#define dbgv(x) { cerr << #x << ": {"; for(int i = 0; i < x.size(); ++i) { if(i) cerr << ", "; cerr << x[i]; } cerr << "}" << endl; }

vector< vector<int> > g;

set< vector<int> > all;
vector<int> cur;
int n;
void dfs(int start, int u, int pred = -1)
{
	cur.pb(u);

	if(u == start)
	{
		for(int i = 0; i < g[u].size(); ++i)
			if(g[u][i] > u)
				dfs(start, g[u][i], start);
	} else
	{
		int id = -1;
		int dif = 999999;
		bool found = false;
		for(int i = 0; i < g[u].size(); ++i) if(g[u][i] != pred && (g[u][i] > u || g[u][i] == start))
		{
			int v = g[u][i];
			
			int nstart = start;
			while(nstart < v)
				nstart += n;
			if(nstart - v < dif)
			{
				if(v == nstart)
					found = true;
				id = v;
				dif = nstart - v;
			}
		}
		if(found)
		{
			all.insert(cur);
		} else if(id != -1)
		{
			while(id >= n)
				id -= n;
			dfs(start, id);
		}
	}
	cur.pop_back();
}

int main()
{
	freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
	int t;
	scanf("%d", &t);
	for(int z = 0; z < t; ++z)
	{
		scanf("%d", &n);
		int m; scanf("%d", &m);
		vector<int> u(m), v(m);
		for(int i = 0; i < m; ++i)
			scanf("%d", &u[i]);
		for(int i = 0; i < m; ++i)
			scanf("%d", &v[i]);

		g.clear();
		g.resize(n);
		for(int i = 0; i < n; ++i)
		{
			g[i].pb((i + 1) % n);
			g[(i + 1) % n].pb(i);
		}
		for(int i = 0; i < n; ++i)
			sort(g[i].begin(), g[i].end());
		for(int i = 0; i < m; ++i)
		{
			u[i]--;
			v[i]--;
			g[u[i]].pb(v[i]);
			g[v[i]].pb(u[i]);
		}
		
		all.clear();
		cur.clear();
		for(int i = 0; i < n; ++i)
			dfs(i, i);

		vector< vector<int> > p;
		for(set< vector<int> > :: iterator it = all.begin(); it != all.end(); ++it)
			p.pb(*it);

		vector<int> res(n, 1);
		int best = 1;
		for(int mask = 0; mask < (1 << (2 * n)); ++mask)
		{
			vector<int> col(n);
			set<int> r;
			for(int j = 0; j < n; ++j)
			{
				col[j] = (mask >> (2 * j)) & 3;
				r.insert(col[j]);
			}
			int cols = r.size();

			bool ok = true;
			for(int j = 0; j < p.size(); ++j)
			{
				set<int> used;
				for(int k = 0; k < p[j].size(); ++k)
					used.insert(col[p[j][k]]);
				if(used.size() != cols)
				{
					ok = false;
					break;
				}
			}
			if(ok)
			{
				if(cols > best)
				{
					best = cols;
					res = col;
				}
			}
		}

		cout << "Case #" << z + 1 << ": " << best << endl;
		for(int i = 0; i < n; ++i)
		{
			if(i) printf(" ");
			if(res[i] + 1 > best)
				cerr << "O_0" << endl;
			printf("%d", res[i] + 1);
		}
		printf("\n");



	}
	return 0;
}
#endif

