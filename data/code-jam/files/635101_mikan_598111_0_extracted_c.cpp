#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<bitset>
#include<gmpxx.h>

#define mp make_pair
#define pb push_back
#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,n-1)

typedef long long ll;
//typedef pair<int,int> pint;
typedef mpz_class BigInt;

#define mod 100003

int c[510][510];

using namespace std;
vector<vector<int> > g;
int n;
int get(int a,int b){
	if(g[a][b]!=-1)return g[a][b];
	if(b>n) return 0;
	if(b==n) return 1;
	g[a][b]=0;
	repp(i,2*b-a,n){
		g[a][b]+=get(b,i)*c[i-b-1][b-a-1];
		g[a][b]%=mod;
	}
	g[a][b]%=mod;
	return g[a][b];
}

int main(){
	rep(nn,505)c[nn][0]=c[nn][nn]=1;
	repp(nn,2,505)repp(i,1,nn-1){
		c[nn][i]=(c[nn-1][i-1]+c[nn-1][i])%mod;
	}
	int t;
	cin>>t;
	repp(cas,1,t){
		g.assign(1000,vector<int>(1000,-1));
		cin>>n;
		int ans=0;
		repp(i,2,n)ans=(ans+get(1,i))%mod;
		printf("Case #%d: %d\n",cas,ans);
	}

	return 0;
}
