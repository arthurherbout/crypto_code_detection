#include <iostream>
#include <cstdio>
#include <cmath>
#include <string>

using namespace std;

int pre[5005];
int next[5005];
int K[5005];
int main(void)
{
	freopen("C-small-attempt0.in","r",stdin);
	freopen("out","w",stdout);
	int cases ;
	scanf("%d",&cases);
	int f = 0;
	while( cases -- )
	{
		int n ,i;
		scanf("%d",&n) ;
		for(i = 1; i <= n ; i ++)
		{
			pre[i] = i - 1;
		}
		pre[1] = n ;
		for(i = 1; i <= n ; i ++)
		{
			next[i] = i + 1;
		}
		next[n] = 1;
		int current = 1;
		for(i = 1 ; i <= n ; i ++)
		{
			int num = 0 ; 
			for( ; ; current = next[current])
			{
				num ++;
				if(num == i)
				{
					int temp = current ;
					next[pre[temp]] = next[current] ;
					pre[next[current]] = pre[temp];
					K[current] = i;
					current = next[current];
					break;
				}
			}
		}
		int q;
		scanf("%d",&q);
		int temp ;
		printf("Case #%d:",++f);
		for(i = 0 ; i < q ; i ++)
		{
			scanf("%d",&temp);
			printf(" %d",K[temp]);
		}
		printf("\n");
	}
	return 0;
}