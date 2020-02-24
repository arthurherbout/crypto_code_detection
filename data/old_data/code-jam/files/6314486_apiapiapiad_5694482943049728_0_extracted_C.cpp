#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cassert>
using namespace std;
#define rep(i,a,n) for (int i=a;i<n;i++)
#define per(i,a,n) for (int i=n-1;i>=a;i--)
#define pb push_back
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define fi first
#define se second
#define SZ(x) ((int)(x).size())
typedef vector<int> VI;
typedef long long ll;
typedef pair<int,int> PII;
const ll mod=1000000007;
ll powmod(ll a,ll b) {ll res=1;a%=mod; assert(b>=0); for(;b;b>>=1){if(b&1)res=res*a%mod;a=a*a%mod;}return res;}
// head

/*
typedef double db;

map<int,vector<int> > hs;
int G[30][30];
db g[30][30];
int M=22;
int gauss(int n) {
	db ret=1;
	rep(i,0,n) {
		int p=i;
		rep(j,i+1,n) if (fabs(g[j][i])>=fabs(g[p][i])) p=j;
		rep(j,i,n+1) swap(g[i][j],g[p][j]);
		if (fabs(g[i][i])<=1e-9) {
			return 0;
		}
		ret=ret*g[i][i];
		rep(k,i+1,n+1) g[i][k]/=g[i][i]; g[i][i]=1;
		rep(j,i+1,n) {
			db cof=-g[j][i];
			rep(k,i,n+1) g[j][k]+=cof*g[i][k];
		}
	}
	ret=max(ret,-ret);
	if (ret>1e5) return 100000;
	return (int)(ret+0.1);
}

int calc() {
	rep(i,0,M) rep(j,0,M) g[i][j]=0;
	rep(i,0,M) rep(j,0,M) if (G[i][j]){
		g[i][j]=-1; g[i][i]+=1;
	}
	return gauss(M-1);
}
void gao() {
	while (SZ(hs)<9999) {
		rep(i,0,M) rep(j,0,M) G[i][j]=0;
		while (1) {
			int u=rand()%M,v=rand()%M;
			if (G[u][v]==1||u==v) continue;
			G[u][v]=G[v][u]=1;
			int w=calc();
		//	printf("%d\n",w);
			if (w>10000) break;
			if (hs.count(w)) continue;
			else {
				VI v;
				rep(i,0,M) rep(j,0,M) v.pb(g[i][j]);
				hs[w]=v;
			}
		}
		printf("%d\n",SZ(hs));
	}
	rep(i,3,10001) if (!hs.count(i)) printf("miss %d\n",i);
}
*/
int _,__,k;
char s[10100];
vector<string> board[10100];
int main() {
	FILE *f=fopen("cc.out","r");
	rep(i,0,10000) {
		fscanf(f,"%d",&k);
		rep(j,0,22) {
			fscanf(f,"%s",&s);
			board[k].pb(string(s));
		}
	}
	for (scanf("%d",&_);_;_--) {
		printf("Case #%d: ",++__);
		fprintf(stderr,"Case #%d\n",__);
		scanf("%d",&k);
		puts("22");
		rep(i,0,22) printf("%s\n",board[k][i].c_str());
	}
	
}
