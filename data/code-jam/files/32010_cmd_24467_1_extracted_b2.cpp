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
spaint a[333];

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; scanf("%d", &t);
	for(int z = 1; z <= t; ++z)
	{
		int n; cin >> n;
		map<string, int> col;
		//vector<spaint> a;
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
			a[i].a = aa;
			a[i].b = b;
			a[i].c = id;
			//a.push_back(spaint(id, aa, b));
		}
		sort(a, a+n, cmpPaints);
/*		vector<vector<int> > dp(11111, vector<int>(n+1, inf));
		dp[0][0] = 0;

		for(int i = 1; i <= 10000; ++i)
			for(int j = 1; j <= n; ++j)
			{
				
			}
*/
		int ans = n + 1;
		int m = col.size();
		for(int i = 0; i < m; ++i)
			for(int j = i; j < m; ++j)
				for(int k = j; k < m; ++k)
				{
					int left = 0, right = 0;
					bool fl = true;
					int mr = -1;
					int cnt = 0;
					for(int l = 0; l < n; ++l) if(a[l].c == i || a[l].c == j || a[l].c == k)
					{
						if(a[l].a - 1 > right)
						{
							if(mr < 0)
							{
								fl = false;
								break;
							}
							else
							{
								right = mr;
								cnt++;
							}
							l--;
							mr = -1;
						} else
						{
							mr = max(mr, a[l].b);
						}
					}
					if(right != 10000)
					{
						if(mr >= 0)
							right = max(right, mr),
							cnt++;
				
					}
					if(right < 10000)
						fl = false;
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