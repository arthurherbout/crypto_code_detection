#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <string.h>
#include <string>
#include <map>
using namespace std;

__int64 a[500002], in[500002], dp[500002], hash[500002];
__int64 data[500002], T[500002]; 

inline int lowbit(int t) 
{ 
	return t & (-t); 
} 

inline __int64 sum(int end) 
{ 
	__int64 sum = 0; 
	while(end > 0) 
	{ 
		sum += T[end]; 
		end -= lowbit(end); 
	} 
	return sum; 
} 

inline void pluss(int pos, __int64 num, int count) 
{ 
  while(pos <= count) 
  { 
      T[pos] += num; 
      pos += lowbit(pos); 
  } 
} 

map<int, int> ma;
map<int, int>::iterator p;

int main()
{
	int i, n, c, ca = 0, t, m; __int64 x, y, z, ans;
	freopen("c:\\C2.in", "r", stdin);
	freopen("c:\\test6.out", "w+", stdout);
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d%d%I64d%I64d%I64d", &n, &m, &x, &y, &z);
		for(i = 0;i < m; i++) scanf("%I64d", &a[i]);
		for(i = 0;i <= n-1; i++)
		{
			in[i+1] = a[i%m];
			a[i%m] = (x*a[i%m] + y*(i+1))%z;
		}

		for(i = 1;i <= n; i++) hash[i] = in[i];
		sort(hash+1, hash+n+1);
		c = 1;
		ma[hash[1]] = 1;
		for(i = 2;i <= n; i++)
		{
			if(hash[i] != hash[i-1]) ma[hash[i]] = ++c;
		}
		for(i = 1;i <= n; i++)
		{
			in[i] = ma[in[i]];
		}
		memset(data, 0, sizeof(data));
		memset(T, 0, sizeof(T));

		ans = 0;
		dp[1] = 1;
		data[in[1]] += dp[1];
		pluss(in[1], dp[1], n);

		for(i = 2;i <= n; i++)
		{
			dp[i] = 1;
			dp[i] += sum(in[i]-1)%1000000007;
			dp[i] %= 1000000007;
			__int64 re = data[in[i]];
			__int64 tmp = (data[in[i]] + dp[i]) % 1000000007;
			pluss(in[i], tmp-re, n);
			data[in[i]] = tmp;
		}
		ans = sum(n)%1000000007;
		printf("Case #%d: %I64d\n", ++ca, ans);
	}
	return 0;
}
