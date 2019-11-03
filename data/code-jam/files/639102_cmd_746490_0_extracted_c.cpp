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



int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; scanf("%d", &t);
	for(int z = 0; z < t; ++z)
	{
		int n; scanf("%d", &n);
		map<int, int> d;
		
		set<pii > w;
		for(int i = 0; i < n; ++i)
		{
			int x, q; scanf("%d%d", &x, &q);
			d[x] += q;
			if(q > 1)
				w.insert(mp(-q, x));
		}
		int cnt = 0;		
		while(w.size())
		{
			pii cur = *w.begin();
			w.erase(w.begin());
			if(d[cur.Y] <= 1)
				continue;
			cnt++;

			d[cur.Y] -= 2;

			if(d[cur.Y] > 1)
				w.insert(mp(cur.X + 2, cur.Y));

			w.erase(mp(-d[cur.Y - 1], cur.Y - 1));
			w.erase(mp(-d[cur.Y + 1], cur.Y + 1));
			w.insert(mp(-d[cur.Y - 1], cur.Y - 1));
			w.insert(mp(-d[cur.Y + 1], cur.Y + 1));
			d[cur.Y - 1]++;
			d[cur.Y + 1]++;
			if(d[cur.Y - 1] > 1)
				w.insert(mp(-d[cur.Y - 1], cur.Y - 1));
			if(d[cur.Y + 1] > 1)
				w.insert(mp(-d[cur.Y + 1], cur.Y + 1));			
		}
		printf("Case #%d: %d\n", z + 1, cnt);
		
	}
	return 0;
}
#endif