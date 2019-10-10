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

#define prev fuck
const int N=1010000,M=20000;

int n,x;
vector<int> e[N];
const int inf=0x20202020;
typedef int flowt;
namespace flow {
	const int M=100000,N=10000;
	int y[M],nxt[M],gap[N],fst[N],c[N],pre[N],q[N],dis[N];
	flowt f[M];
	int S,T,tot,Tn;
	void init(int s,int t,int tn) {
		tot=1; assert(tn<N);
		rep(i,0,tn) fst[i]=0;
		S=s;T=t;Tn=tn;
	}
	void add(int u,int v,flowt c1,flowt c2=0) {
		tot++;y[tot]=v;f[tot]=c1;nxt[tot]=fst[u];fst[u]=tot;
		tot++;y[tot]=u;f[tot]=c2;nxt[tot]=fst[v];fst[v]=tot;
	}
	flowt sap() {
		int u=S,t=1;flowt flow=0;
		rep(i,0,Tn) c[i]=fst[i],dis[i]=Tn,gap[i]=0;
		q[0]=T;dis[T]=0;pre[S]=0;
		rep(i,0,t) {
			int u=q[i];
			for (int j=fst[u];j;j=nxt[j]) if (dis[y[j]]>dis[u]+1&&f[j^1]) 
				q[t++]=y[j],dis[y[j]]=dis[u]+1;
		}
		rep(i,0,Tn) gap[dis[i]]++;
		while (dis[S]<=Tn) {
			while (c[u]&&(!f[c[u]]||dis[y[c[u]]]+1!=dis[u])) c[u]=nxt[c[u]];
			if (c[u]) {
				pre[y[c[u]]]=c[u]^1;
				u=y[c[u]];
				if (u==T) {
					flowt minf=inf;
					for (int p=pre[T];p;p=pre[y[p]]) minf=min(minf,f[p^1]);
					for (int p=pre[T];p;p=pre[y[p]]) f[p^1]-=minf,f[p]+=minf;
					flow+=minf;u=S;
				}
			} else {
				if (!(--gap[dis[u]])) break;
				int mind=Tn;
				c[u]=fst[u];
				for (int j=fst[u];j;j=nxt[j]) if (f[j]&&dis[y[j]]<mind) 
					mind=dis[y[j]],c[u]=j;
				dis[u]=mind+1;
				gap[dis[u]]++;
				if (u!=S) u=y[pre[u]];
			}
		}
		return flow;
	}
};

bool check(int l,int r) {
	if (r<l) return 1;
/*	rep(i,l,r+1) {
		int cnt=0;
		rep(j,0,n) {
			bool val=0;
			for (auto p:e[j]) if (p>=i&&p<=r) val=1;
			cnt+=val;
		}
		if (cnt<r-i+1) return 0;
	}
	return 1;*/
	int s=n+r-l+1;
	flow::init(s,s+1,s+2);
	rep(i,0,n) {
		flow::add(s,i,1);
		for (auto p:e[i]) if (p>=l&&p<=r) flow::add(i,p-l+n,1);
	}
	rep(i,l,r+1) flow::add(i-l+n,s+1,1);
	return flow::sap()==r-l+1;
/*	rep(pl,l,r+1) rep(pr,pl,r+1) {
		int cnt=0;
		rep(j,0,n) {
			bool val=0;
			for (auto p:e[j]) if (p>=pl&&p<=pr) val=1;
			cnt+=val;
		}
		if (cnt<pr-pl+1) return 0;
	}*/
	return 1;
}

void solve() {
	scanf("%d",&n);
	rep(i,0,n) e[i].clear();
	rep(i,0,n) {
		rep(j,0,6) scanf("%d",&x),e[i].pb(x);
	}
	int r=0,ret=0;
	rep(i,1,M+1) {
		while (check(i,r+1)) ++r;
		ret=max(ret,r-i+1);
	}
	printf("%d\n",ret);
}

int _,__;
int main() {
	for (scanf("%d",&_);_;_--) {
		printf("Case #%d: ",++__);
		fprintf(stderr,"Case #%d\n",__);
		solve();
	}
}
