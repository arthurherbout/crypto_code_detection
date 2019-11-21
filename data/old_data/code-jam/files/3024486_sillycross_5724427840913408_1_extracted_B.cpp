#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <cstring>

using namespace std;

typedef long long LL;
typedef unsigned long long ULL;

#define SIZE(x) (int((x).size()))
#define rep(i,l,r) for (int i=(l); i<=(r); i++)
#define repd(i,r,l) for (int i=(r); i>=(l); i--)
#define rept(i,c) for (typeof((c).begin()) i=(c).begin(); i!=(c).end(); i++)

#ifndef ONLINE_JUDGE
#define debug(x) { cerr<<#x<<" = "<<(x)<<endl; }
#else
#define debug(x) {}
#endif

#define maxn 110
#define INF 1000000000

int hp[maxn], money[maxn], dp[maxn][210][maxn*12];

void su(int &a, int b) { a=max(a,b); }

void lemon()
{
	int n,arrow,tower; scanf("%d%d%d",&arrow,&tower,&n);
	rep(i,1,n) scanf("%d%d",&hp[i],&money[i]);
	hp[n+1]=0;
	
	//dp[i][HP][turns] now shooting monster i, which has HP hp left, player has 'turns' turns to shoot other monsters
	rep(i,0,n+1) rep(j,0,200) rep(k,0,12*n) dp[i][j][k]=-INF;
	dp[1][hp[1]][1]=0;
	rep(i,1,n)
		repd(rm,hp[i],1)
			rep(turns,0,12*i)
			{
				if (dp[i][rm][turns]<0) continue;
				
				if (rm==hp[i])
				{
					int need=rm/arrow; if (rm%arrow>0) need++;
					if (turns>=need)
					{
						su(dp[i+1][hp[i+1]][turns-need],dp[i][rm][turns]+money[i]);
					}
				}
				
				if (rm<=tower)
				{
					su(dp[i+1][hp[i+1]][turns+1],dp[i][rm][turns]);
					continue;
				}
				if (rm-tower<=arrow)
				{
					su(dp[i+1][hp[i+1]][turns],dp[i][rm][turns]+money[i]);
				}
				else
				{
					su(dp[i][rm-tower-arrow][turns],dp[i][rm][turns]);
				}
				su(dp[i][rm-tower][turns+1],dp[i][rm][turns]);
				
				int need=(rm-tower)/arrow; if ((rm-tower)%arrow>0) need++;
				if (turns+1>=need)
				{
					su(dp[i+1][hp[i+1]][turns+1-need],dp[i][rm][turns]+money[i]);
				}
			}
	
	//dp[n+1][0][*]
	int final=0;
	rep(i,0,(n+1)*12) final=max(final,dp[n+1][0][i]);
	printf("%d\n",final);
}

int main()
{
	ios::sync_with_stdio(true);
	#ifndef ONLINE_JUDGE
		freopen("B.in","r",stdin);
	#endif
	int tcase; scanf("%d",&tcase);
	rep(nowcase,1,tcase) 
	{
		printf("Case #%d: ",nowcase);
		lemon();
	}
	return 0;
}

