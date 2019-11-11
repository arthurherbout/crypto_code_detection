#include <string>
#include <queue>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
using namespace std;
int N,M;
char Data[55][55];
int Check[50][50];
int From[50][50];
int Dis[50][50];
int findlen(int y,int x)
{
	int nc[55][55];
	int ds[55][55];
	int q,w;
	for (q=0;q<N;q++) for (w=0;w<M;w++) nc[q][w]=0;
	queue<int>Q;
	Q.push(y*M+x);
	nc[y][x]=1;
	ds[y][x]=0;
	while (!Q.empty())
	{
		y=Q.front()/M;
		x=Q.front()%M;
		Q.pop();
		int yp[4]={-1,1,0,0};
		int xp[4]={0,0,-1,1};
		for (q=0;q<4;q++)
		{
			int ny=y+yp[q],nx=x+xp[q];
			if (ny<0 || ny>=N || nx<0 || nx>=M) continue;
			if (nc[ny][nx]) continue;
			if (Data[ny][nx]=='.') continue;
			ds[ny][nx]=ds[y][x]+1;
			nc[ny][nx]=1;
			Q.push(ny*M+nx);
		}
	}
	return ds[0][0];
}
int main()
{
	srand((unsigned)time(NULL));
	freopen("D-small-attempt0.in","r",stdin);
	freopen("D-small.out","w",stdout);
	int kase,T;
	scanf("%d",&T);
	//T=0;//rand();
	for (kase=1;kase<=T;kase++)
	{
		vector<int>V;
		scanf("%d %d",&N,&M);
		int q,w;
		char TEMP[50];
		for (q=0;q<N;q++) 
		{
			scanf("%s",Data[q]);
			for (w=0;w<M;w++) 
				if (Data[q][w]=='T')
					V.push_back(q*M+w);
		}
		queue<int>Q;
		for (q=0;q<V.size();q++) 
		{
			Q.push(V[q]);
			Check[V[q]/M][V[q]%M]=kase;
			Dis[V[q]/M][V[q]%M]=0;
		}
		int yp[4]={-1,1,0,0};
		int xp[4]={0,0,-1,1};
		while (!Q.empty())
		{
			int y=Q.front()/M,x=Q.front()%M; 
			Q.pop();
			for (q=0;q<4;q++)
			{
				int ny=y+yp[q],nx=x+xp[q];
				if (ny<0 || ny>=N || nx<0 || nx>=M) continue;
				if (Check[ny][nx]>=kase) continue;
				if (Data[ny][nx]=='.') continue;
				Check[ny][nx]=kase;
				Q.push( ny*M + nx );
				From[ny][nx]=y*M+x;
				Dis[ny][nx]=Dis[y][x]+1;
			}
		}
		int ret=0;
		for (q=0;q<N;q++) for (w=0;w<M;w++) if (Data[q][w]!='.') ret+=Dis[q][w];
		if (V.size()==2)
		{
			int len=findlen(V[1]/M,V[1]%M);
			for (q=len;q>0;q-=2) ret+=q;
		}
		printf("Case #%d: %d\n",kase,ret);
	}
	return 0;
}