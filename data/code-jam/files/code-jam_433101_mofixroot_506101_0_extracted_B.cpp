#include<iostream>
#include<algorithm>

using namespace std;

const int maxn = 100;
int n;
int gcd(int a, int b)
{
	while(b!=0)
	{
		int t = a;
		a = b;
		b = t%b;
	}
	return a;
}

int d[maxn];

int main()
{
	freopen("B-small-attempt4.in", "r", stdin);
	freopen("out.txt", "w", stdout);
	int cas, icas;
	int i;
	scanf("%d", &cas);
	for(icas = 1; icas <= cas; icas++)
	{
		scanf("%d", &n);
		for(i = 1; i <= n; i++)
		{
			scanf("%d", &d[i]);
		}
		sort(d+1, d+1+n);
		int mind;
		if(n==2)
		{
			mind = d[2] - d[1];
			for(i = 0; i <= 100000000; i++)
			{
				if((d[1]+i)%mind==0 && (d[2]+i)%mind==0)
				{
					printf("Case #%d: %d\n", icas, i);
					break;
				}
			}
		}
		if(n==3)
		{
			int d1 = d[2]-d[1];
			int d2 = d[3]-d[2];
			int d3 = d[3]-d[1];
			mind = gcd(d1, d2);
			mind = gcd(mind, d3);
			for(i = 0; i <= 100000000; i++)
			{
				if((d[1]+i)%mind==0 && (d[2]+i)%mind==0 &&(d[3]+i)%mind==0)
				{
					printf("Case #%d: %d\n", icas, i);
					break;
				}
			}
		}
	}
	return 0;
}