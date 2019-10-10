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
using namespace std;
int vv[400];
int vis[20][20];
int t,T;
int r,c,n;
int ans;

int check()
{
	int i,j;
	int a = 0;
	for(i = 1; i <= r;i ++)
		for(j = 1; j <= c; j++)
		{
			if( vis[i][j])
			{
				if(i + 1 <= r)if(vis[i+1][j])a++;
				if(j + 1 <= c)if(vis[i][j+1])a++;
			}
		}
	return a;
}

int dfs()
{
	long k = (2 << (r*c)) ;
	int i,x,y,cont = 0;
	
	for(; k >=1 ;k--)
	{
		long t = k;
		cont = 0;
		i = 0;
		while(t)
		{
			i++;
			vv[i] = t&1;
			if(vv[i-1])cont++;
			
			t = t >> 1;
		}
		while( i < r * c)
		{
			i++;
			vv[i] = 0;
		}
		
		if(cont == n)
		{
		i = 1;
		for(x = 1; x <= r; x++){
			for(y = 1; y <= c; y++)
			{
				vis[x][y] = vv[i];
				i++;
			}
			}

				int M = check();
				if(M  < ans)ans = M;
	
	}
	}
}
			

int main()
{
	freopen("1.in","r",stdin);
	freopen("1.out","w",stdout);
	scanf("%d", &T);
	for(t = 1; t <= T; t++)
	{
		scanf("%d%d%d", &r,&c,&n);
		ans = 1000;
memset(vis,0,sizeof(vis));
		if(n == 0){printf("Case #%d: 0\n",t);continue;}
		if(n == r * c){printf("Case #%d: %d\n", t, (n*4 - 2*r - 2*c)/2);continue;}

		dfs();

		printf("Case #%d: %d\n",t,ans);
		}

	return 0;
}
