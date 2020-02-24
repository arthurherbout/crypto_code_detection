

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <time.h>
#include <set>
using namespace std;

__int64 a[10002], in[10002], dp[10002];

int main()
{
	int i, k, l, n, p, c, ca = 0, t, j, m; __int64 x, y, z, ans;
	freopen("c:\\C.in", "r", stdin);
	freopen("c:\\test5.out", "w+", stdout);
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d%d%I64d%I64d%I64d", &n, &m, &x, &y, &z);
		for(i = 0;i < m; i++) scanf("%I64d", &a[i]);
		for(i = 0;i <= n-1; i++)
		{
			in[i] = a[i%m];
			a[i%m] = (x*a[i%m] + y*(i+1))%z;
		}
		ans = 0;
		dp[0] = 1;
		for(i = 1;i <= n-1; i++)
		{
			dp[i] = 1;
			for(j = 0;j < i; j++)
			{
				if(in[j] < in[i])
				{
					dp[i] += dp[j];
					dp[i] %= 1000000007;
				}
			}
		}
		for(i = 0;i <= n-1; i++) {ans += dp[i]; ans %= 1000000007;}
		printf("Case #%d: %I64d\n", ++ca, ans);
	}
	return 0;
}
