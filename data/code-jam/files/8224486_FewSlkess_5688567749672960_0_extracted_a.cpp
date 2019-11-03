#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>
#include <time.h>
#define F 10000000

using namespace std;



long long N;
bool vis[10000000];

struct Node{long long ste;long long v;};
long long ans;

long long fz(long long us)
{
	int h[20],i = 0,j;
	long long t = us;
	while(t)
	{
		h[i] = t % 10;
		t /= 10;
		i++;
	}
	long long ned = 0;
	for(j = 0; j < i; j++)if(h[j])break;
	while(j < i)
	{
		ned *= 10;
		ned += h[j];
		j++;
	}
	return ned;
}

	

int main()
{
	freopen("1.in", "r",stdin);
	freopen("1.out","w", stdout);
	

	
	int T,t;
	scanf("%d", &T);
	for(t = 1; t <= T;t++)
	{

		queue<Node> q;
		scanf("%lld", &N);

	
		while( !q.empty() )q.pop();

		memset(vis, 0, sizeof(vis));


		Node k;
		k.ste = 1; k.v = 1;vis[1] = 1;
		q.push(k);

		while( !q.empty())
		{
			 Node now = q.front();
			q.pop();

			long long us = now.v;
			if(now.ste >= N){ans = now.ste;break;};
			if(now.v == N){ans = now.ste;break;};

			if( !vis[us + 1]) 
			{k.v = us+1; k.ste = now.ste + 1;vis[us + 1] = 1;
			q.push(k);}
			long long zs = fz(us);

			if( !vis[ zs ])
			{
				k.v = zs; k.ste = now.ste + 1;vis[zs] = 1;
				q.push(k);
			}
		}

		printf("Case #%d: %lld\n",t,ans);
	}

return 0;
}
