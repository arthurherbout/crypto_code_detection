#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <cctype>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <stack>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
#include <iostream>
#include <sstream>
using namespace std;

#define size(x) int((x).size())
#define foreach(i,c) for(typeof((c).begin()) i=(c).begin();i!=(c).end();i++)
typedef long long I64; typedef unsigned long long U64;
const double EPS=1e-12;
const int INF=999999999;
typedef vector<int> VI;
typedef vector<string> VS; 

int n,m;
char g[17][16];
int pre,cur;
int f[2][1<<15];
int nQ[2];
int Q[2][1<<15];

void solve()
{
	scanf("%d%d",&n,&m);
	for(int i=0;i<n;i++) scanf("%s",g[i]);

	cur=0;
	nQ[0]=1,Q[0][0]=0;
	f[0][0]=0;

	int ans=0;
	for(int r=0;r<n;r++)
		for(int c=0;c<m;c++) {
			pre=cur,cur=1-pre;
			nQ[cur]=0;
			memset(f[cur],-1,sizeof(f[cur]));

			for(int d=0;d<=1;d++) {
				if(g[r][c]=='#' && d==0) continue;
				if(g[r][c]=='.' && d==1) continue;

				for(int i=0;i<nQ[pre];i++) {
					int v=Q[pre][i];

					int dL,dU;
					if(c>0) dL=(v>>(c-1))&1; else dL=0;
					dU=(v>>c)&1;

					int w2=f[pre][v];
					if(d) {
						w2+=4;
						if(dL) w2-=2;
						if(dU) w2-=2;
					}

					int v2=v;
					if(dU!=d) v2^=1<<c;

					if(f[cur][v2]==-1) Q[cur][nQ[cur]++]=v2;
					f[cur][v2]>?=w2;

					if(r==n-1 && c==m-1) ans>?=w2;
				}
			}
		}

	printf("%d",ans);
}

int main()
{	
	int T;
	scanf("%d",&T);
	for(int t=1;t<=T;t++) {
		printf("Case #%d: ",t);
		solve();
		printf("\n");
	}


	return 0;
}
