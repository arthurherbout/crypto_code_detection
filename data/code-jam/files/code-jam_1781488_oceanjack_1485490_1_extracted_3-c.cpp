#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;
typedef long long ll;
ll dp[123][123],ha[123][123],hb[123][123];
struct Node
{
	ll tmp,val;
} a[123], b[123];
int main()
{
	freopen("in.in","r",stdin);
	freopen("out.txt","w",stdout);
	int c, sc = 1;
	for(scanf("%d",&c); c; --c, ++sc)
	{
		int n, m;
		memset(dp, 0, sizeof(dp));
		memset(ha, 0, sizeof(ha));
		memset(hb, 0, sizeof(hb));
		scanf("%d%d",&n,&m);
		for(int i = 0; i < n; ++i)
		{
			scanf("%lld%lld",&a[i].val,&a[i].tmp);
			ha[a[i].tmp][i+1] = a[i].val;
			for(int k = 0; k <= 100; ++k)
				ha[k][i+1] += ha[k][i];
		}
		for(int i = 0; i < m; ++i)
		{
			scanf("%lld%lld",&b[i].val,&b[i].tmp);
			hb[b[i].tmp][i+1] = b[i].val;
			for(int k = 0; k <= 100; ++k)
				hb[k][i+1] += hb[k][i];
		}
		for(int i = 1; i <= n; ++i)
			for(int j = 1; j <= m; ++j)
			{
				if(a[i-1].tmp == b[j-1].tmp)
				{
					for(int x = 0; x < i; ++x)
						for(int y = 0; y < j; ++y)
							dp[i][j] = max(dp[i][j], dp[x][y] + min(ha[a[i-1].tmp][i] - ha[a[i-1].tmp][x], hb[b[j-1].tmp][j] - hb[b[j-1].tmp][y]));
				}
				else
					dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
			}
		ll ans = -1;
		for(int i = 0; i <= n; ++i)
			for(int j = 0; j <= m; ++j)
				ans = max(ans, dp[i][j]);
		printf("Case #%d: %lld\n",sc, ans);
	}
	return 0;
}
