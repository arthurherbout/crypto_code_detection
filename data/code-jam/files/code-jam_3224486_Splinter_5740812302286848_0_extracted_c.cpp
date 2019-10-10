#include <bits/stdc++.h>
using namespace std;
int n;
int s , x[1001] , y[1001] , z[1001];
int vx[1001] , vy[1001] , vz[1001];

double dist(int a , int b)
{
	return 
	sqrt(double(x[a] - x[b]) * (x[a] - x[b]) + double(y[a] - y[b]) * (y[a] - y[b]) +
	double(z[a] - z[b]) * (z[a] - z[b]));}

int p[1001];
int getf(int x){return x == p[x] ? x : p[x] = getf(p[x]);}
void merge(int a , int b)
{
	int pa = getf(a) , pb = getf(b);
	if(pa == pb)return;
	p[pa] = pb;
}

int main()
{
	int test;scanf("%d" , &test);
	for(int _ = 1 ; _ <= test ; _++)
	{
		scanf("%d" , &n);
		scanf("%d" , &s);
		for(int i = 1;  i <= n ; i++)
			scanf("%d%d%d%d%d%d" , &x[i] , &y[i] , &z[i] , &vx[i] , &vy[i] , &vz[i]);
		double ll = 0 , rr = 10000 , mid;
		for(int tt = 1 ; tt <= 50 ; tt++)
		{
			mid = (ll + rr) / 2.0;
			for(int i = 1 ; i <= n ; i++)
				p[i] = i;
			for(int i = 1 ; i <= n ; i++)
			for(int j = 1 ; j <= n ; j++)
			{
				if(i == j)continue;
				if(dist(i , j) <= mid)
					merge(i , j);
			}
			if(getf(1) == getf(2))
				rr = mid;
			else ll = mid;
		}
		printf("Case #%d: %.7f\n" , _ , (ll + rr) / 2.0);
	}
	return 0;
}