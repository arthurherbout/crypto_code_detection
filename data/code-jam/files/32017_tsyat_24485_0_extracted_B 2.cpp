#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;


int A , B ,P;
bool bl[1005];
int prime[1005] ,cnt;
void init()
{
	int i , j;
	for(i = 2 ; i <= 32 ; i ++)
	{
		if( bl[i] == 0)
		{
			for(j = 2 * i ; j <= 1000 ; j += i)
			{
				bl[j] = 1;
			}
		}
	}
	for(i = 2 ; i <= 1000 ; i ++)
	{
		if(bl[i] == 0)
		{
			prime[cnt++] = i;
		}
	}
}
int bing[1005];
int find(int n)
{
	if(bing[n] == n) return n ;
	return bing[n] = find(bing[n]);
}
int work()
{
	int i ,j ;
	for(i = A ; i <= B ; i ++)
		bing[i] = i;
	int startpos ;
	for( i = 0 ; i < cnt ; i ++)
	{
		if( prime[i] >= P)
		{
			startpos = i;
			break;
		}
	}
	for(i = startpos ; i < cnt ; i ++)
	{
		int start = (A / prime[i] ) ;
		if(A % prime[i] != 0) start ++;
		start *= prime[i];
		for(j = start ; j <= B ; j += prime[i])
		{
			int fa = find(j) ;
			int fb = find(start);
			bing[fa] = fb;
		}
	}
	int ans = 0 ;
	for( i = A  ; i <= B ; i ++)
	{
		if( i== bing[i])
			ans ++;
	}
	return ans;
}


int main(void)
{
	int cases ;
	init();
	int f = 0;
	freopen("B-small-attempt.in","r",stdin);
	freopen("out","w",stdout);
	scanf("%d",&cases);
	while( cases -- )
	{
		scanf("%d %d %d",&A,&B,&P);
		printf("Case #%d: %d\n",++f,work());
	}
	return 0;
}