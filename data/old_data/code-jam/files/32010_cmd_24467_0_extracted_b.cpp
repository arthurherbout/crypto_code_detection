#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <map>
#include <stack>
#include <string>
#include <functional>
using namespace std;


typedef long long LL;
#define mp make_pair
#define pb push_back
#define X first
#define Y second
const int inf = 999999;

struct spaint
{
	int c, a, b;
	spaint() {}
	spaint(int cc, int aa, int bb) { c = cc; a = aa; b = bb; }
};

bool cmpPaints(const spaint &a, const spaint &b)
{
	if(a.a < b.a) return true;
	if(a.a == b.a && a.b < b.b) return true;
	return false;
}


int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; scanf("%d", &t);
	for(int z = 1; z <= t; ++z)
	{
		int n; cin >> n;
		map<string, int> col;
		vector<spaint> a;
		for(int i = 0; i < n; ++i)
		{
			string c;
			int aa, b;
			cin >> c >> aa >> b;
			int id;
			if(!col.count(c))
			{
				int sz = col.size();
				col[c] = sz;
			}
			id = col[c];
			//a[i].a = a;
			//a[i].b = b;
			//a[i].c = id;
			a.push_back(spaint(id, aa, b));
		}
		sort(a.begin(), a.end(), cmpPaints);
/*		vector<vector<int> > dp(11111, vector<int>(n+1, inf));
		dp[0][0] = 0;

		for(int i = 1; i <= 10000; ++i)
			for(int j = 1; j <= n; ++j)
			{
				
			}
*/
		int ans = n + 1;
		for(int i = 0; i < (1<<n); ++i)
		{
			set<int> q;
			int cnt = 0;
			for(int j = 0; j < n; ++j)
				if(i & (1<<j))
					q.insert(a[j].c),
					cnt++;
			if(q.size() > 3) continue;
			
			int left = 0, right = 0;
			bool fl = true;
			for(int j = 0; j < n; ++j)
				if(i & (1<<j))
				{
					if(a[j].a -1 > right )
					{
						fl = false;
						break;
					} else
						right = max(right, a[j].b);
				}
			if(right < 10000)
				fl = false;
			for(int j = 0; j < n; ++j) if(i && (1<<j))
				for(int k = 0; k < n; ++k) if(i && (1<<k))
				{
					//printf("%d - %d\n", j, k);
					if(a[j].a >= a[k].a && a[j].a <= a[k].b ||
						a[k].a >= a[j].a && a[k].a <= a[k].b ||
						a[j].b >= a[k].a && a[j].b <= a[k].b ||
						a[k].b >= a[j].a && a[k].b <= a[k].b )
					{
					//	if(a[j].c != a[k].c)
						//	fl = false;
					}
				}
			if(fl)
				ans = min(ans, cnt);
		}
		if(ans == n + 1)
			printf("Case #%d: IMPOSSIBLE\n", z);
		else
			printf("Case #%d: %d\n", z, ans);
	}
	return 0;
}