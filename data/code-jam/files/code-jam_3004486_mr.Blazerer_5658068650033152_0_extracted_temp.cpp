#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <list>
#include <stack>
#include <set>
#include <map>

using namespace std;
int n,m,k;
bool c[21][21];
bool can[21][21];
queue<pair<int,int> > Q;
bool test()
{
	for (int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			can[i][j]=0;
		}
	}
	for (int i=0;i<n;i++)
		Q.push(make_pair(i,0));
	for (int i=0;i<n;i++)
		Q.push(make_pair(i,m-1));
	for (int i=0;i<m;i++)
		Q.push(make_pair(0,i));
	for (int i=0;i<m;i++)
		Q.push(make_pair(n-1,i));

	while (Q.size())
	{
		pair<int,int> curp = Q.front();
		Q.pop();
		int ci = curp.first;
		int cj = curp.second;
		can[ci][cj]=true;
		if (c[ci][cj])
			continue;
		if (ci>0)
		{
			if (can[ci-1][cj]==0)
			{
				can[ci-1][cj]=1;
				Q.push(make_pair(ci-1,cj));
			}
		}
		if (ci<n-1)
		{
			if (can[ci+1][cj]==0)
			{
				can[ci+1][cj]=1;
				Q.push(make_pair(ci+1,cj));
			}
		}
		if (cj>0)
		{
			if (can[ci][cj-1]==0)
			{
				can[ci][cj-1]=1;
				Q.push(make_pair(ci,cj-1));
			}
		}
		if (cj<m-1)
		{
			if (can[ci][cj+1]==0)
			{
				can[ci][cj+1]=1;
				Q.push(make_pair(ci,cj+1));
			}
		}
	}

	int res=0;
	for (int i=0;i<n;i++)
	{
		for(int j=0;j<m;j++)
		{
			if (c[i][j] || !can[i][j])
				res++;
		}
	}
	return res>=k;
}

void go()
{
	cin>>n>>m>>k;
	int res= n*m;
	for (int curmask =0 ;curmask<(1<<(n*m));curmask++)
	{
		int curbit = 1;
		for (int i=0;i<n;i++)
		{
			for (int j=0;j<m;j++)
			{
				c[i][j]= (curbit&curmask);
				curbit*=2;
			}
		}
		if (test())
		{
			int curres=0;
			for (int i=0;i<(n*m);i++)
			{
				if (curmask & (1<<i))
					curres++;
			}
			res = min(curres,res);
		}
	}
	cout<<res;
}
int main()
{	
	freopen("1.txt","r",stdin);
	freopen("2.txt","w",stdout);
	int cases;
	cin>>cases;
	for (int curcase=1;curcase<=cases;curcase++)
	{
		cout<<"Case #"<<curcase<<": ";
		{
			go();
		}
		cout<<"\n";
	}
	return 0;
}
