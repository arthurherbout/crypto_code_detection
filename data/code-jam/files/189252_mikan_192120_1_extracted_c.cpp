#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<bitset>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int,int> pint;
typedef vector<int> vint;

#define mp make_pair
#define pb push_back
#define REP(i,a,b) for(int i=a;i<b;++i)
#define rep(i,n) REP(i,0,n)

int n,p,q;

int main(){
	freopen("c.in","r",stdin);
	freopen("c.out","w",stdout);
	cin>>n;
	REP(cas,1,n+1){
		cin>>p>>q;
		int shu[110];
		rep(i,q)scanf("%d",&shu[i+1]);
		shu[0]=0;shu[q+1]=p+1;
		vector<vint> dp(110,vint(110,-1));
		rep(i,q+1)dp[i][i]=0;
		for(int d=1;d<=q;++d){
			for(int i=0;i<=q-d;++i){
				dp[i][i+d]=shu[i+d+1]-shu[i]-2;
				int mi=1001001001;
				for(int j=0;j<d;++j){
					mi=min(mi,dp[i][i+j]+dp[i+j+1][i+d]);
				}
				dp[i][i+d]+=mi;
			}
		}
		printf("Case #%d: %d\n",cas,dp[0][q]);
	}
	return 0;
}
