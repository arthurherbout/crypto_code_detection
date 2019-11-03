#include <string>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <map>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <queue>
using namespace std;
typedef long long ll;
int Dir[2][2];
int N,M;
int good(int y,int x){ return y>=0 && x>=0 && y<N && x<M; }
int good(ll y,ll x){ return y>=0 && x>=0 && y<N && x<M; }

int ind()
{
	ll a=Dir[0][0],b=Dir[0][1],c=Dir[1][0],d=Dir[1][1];
	if (a*d-b*c) return 1;
	return 0;
}
int Check[1000000];
int main()
{
	srand((unsigned)time(NULL));
	freopen("B-large.in","r",stdin);
	freopen("B-large.out","w",stdout);
	int kase,T,q;
	scanf("%d",&T);
	for (kase=1;kase<=T;kase++)
	{
		scanf("%d %d",&N,&M);
		for (q=0;q<2;q++) scanf("%d %d",Dir[q],Dir[q]+1);
		int y,x;
		ll ret=0;
		scanf("%d %d",&y,&x);
		//if (kase<5) continue;
		if (ind()) //linear independent
		{
			//printf("independent : ");
			int i,j,t=1,bi=0,bj=9999999;
			int iy,ix,jy,jx;
			//set iy ix jy jx
			i=0; iy=y; ix=x;
			j=0; jy=y; jx=x;
			while (true)
			{
				while (good(jy,jx)) 
				{ 
					j++; 
					jy+=Dir[1][0]; 
					jx+=Dir[1][1]; 
				}
				while (j>i && !good(jy-Dir[1][0],jx-Dir[1][1]))
				{
					j--;
					jy-=Dir[1][0];
					jx-=Dir[1][1];
				}
				while (!good(iy,ix) && i<j) 
				{ 
					i++; 
					iy+=Dir[1][0]; 
					ix+=Dir[1][1]; 
				}
				if (i>=bj) break; 
				bi=i;
				bj=j;				
				ret+=j-i;
				if (i==j) break;
				t++;
				iy+=Dir[0][0];
				ix+=Dir[0][1];
				jy+=Dir[0][0];
				jx+=Dir[0][1];
			}
		}
		else if (Dir[0][0]!=0) //it's dependet : check all possible :) and dy!=0
		{
			queue<int>Q;
			Q.push(y);
			Check[y]=kase;
			ret=1;
			while (!Q.empty())
			{
				int n=Q.front();Q.pop();
				for (q=0;q<=1;q++) if ( n+Dir[q][0] >=0 && n+Dir[q][0]<N )
					if (Check[n+Dir[q][0]]<kase)
					{
						//find x
						int dy=n+Dir[q][0]-y;
						int dx=dy*Dir[0][1]/Dir[0][0]; // dx=dy*vx/vy
						if (!good(y+dy,x+dx)) continue;
						ret++;
						Check[n+Dir[q][0]]=kase;
						Q.push(n+Dir[q][0]);
					}
			}
		}
		else //dy=0 but maybe dx!=0
		{
			queue<int>Q;
			Q.push(x);
			Check[x]=kase;
			ret=1;
			while (!Q.empty())
			{
				int n=Q.front();Q.pop();
				for (q=0;q<=1;q++) if ( n+Dir[q][1] >=0 && n+Dir[q][1]<M )
					if (Check[n+Dir[q][1]]<kase)
					{
						//find y
						int dx=n+Dir[q][1]-x;
						int dy=dx*Dir[0][0]/Dir[0][1];
						if (!good(y+dy,x+dx)) continue;
						ret++;
						Check[n+Dir[q][1]]=kase;
						Q.push(n+Dir[q][1]);
					}
			}
		}
		printf("Case #%d: ",kase);
		cout << ret << endl;
	}
	return 0;
}