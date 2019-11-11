/*
	Author       :	Jan
	Problem Name :
	Algorithm    :
	Complexity   :
*/

#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>

using namespace std;

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define CLR(a) memset(a,0,sizeof(a))

#define i64 long long
#define u64 unsigned i64

#define MAX 10

int cases,caseno,n,m,adj[2][MAX][MAX],deg[2][MAX];

void input()
{
	int i,x,y;

	scanf("%d",&n);
	CLR(deg);
	for(i=1;i<n;i++)
	{
		scanf("%d %d",&x,&y);
		adj[0][x][deg[0][x]++]=y;
		adj[0][y][deg[0][y]++]=x;
	}
	scanf("%d",&m);
	for(i=1;i<m;i++)
	{
		scanf("%d %d",&x,&y);
		adj[1][x][deg[1][x]++]=y;
		adj[1][y][deg[1][y]++]=x;
	}
}

bool used[2][MAX];

int avl(int ith,int x)
{
	int cnt=0;

	for(int i=0;i<deg[ith][x];i++) if(!used[ith][adj[ith][x][i]]) cnt++;
	return cnt;
}

bool call(int x,int z)
{
	if(avl(0,x) < avl(1,z)) return false;
	if(avl(1,z)==0) return true;

	int i,j;

	for(j=0;j<deg[1][z];j++) if(!used[1][adj[1][z][j]])
	{
		for(i=0;i<deg[0][x];i++) if(!used[0][adj[0][x][i]])
		{
			used[1][adj[1][z][j]]=true;
			used[0][adj[0][x][i]]=true;

			if(call(adj[0][x][i],adj[1][z][j]))
			{
				if(call(x,z))
				{
					used[1][adj[1][z][j]]=false;
					used[0][adj[0][x][i]]=false;
					return true;
				}
			}

			used[1][adj[1][z][j]]=false;
			used[0][adj[0][x][i]]=false;
		}
	}
	return false;
}

void process()
{
	int i;
	for(i=1;i<=n;i++)
	{
		CLR(used);
		if(call(i,1))
		{
			break;
		}
	}
	if(i<=n) printf("Case #%d: YES\n",++caseno);
	else printf("Case #%d: NO\n",++caseno);
}

int main()
{
	freopen("d.in","r",stdin);
	freopen("d.ans","w",stdout);

	scanf("%d",&cases);
	while(cases--)
	{
		input();
		process();
	}
	return 0;
}
