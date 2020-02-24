// ayy
// ' lamo
#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;
using namespace __gnu_pbds;
typedef long long ll;
typedef long double ld; //CARE
typedef complex<ld> pt;
#define fi first
#define se second
#define pb push_back
const ld eps=1e-8;
const int inf=1e9+99;
const ll linf=1e18+99;
const int P=1e9+7;

const int N=2048;
vector<int> adj[N],ch[N];
map<pair<int,int>,int> es;
bool seen[N];
int z[N],d[N],uu[N],vv[N];
bool istre[N];

void dfs1(int u,int _d=0) {
	d[u]=_d;
	assert(!seen[u]); seen[u]=1;
	for(int v:adj[u]) if(!seen[v]) {
		dfs1(v,_d+1), ch[u].pb(v);
		if(es.count({u,v})) istre[es[{u,v}]]=1; else
		if(es.count({v,u})) istre[es[{v,u}]]=1; else
		assert(0);
	}

}
int dfs2(int u) {
	int _z=0;
	for(int v:ch[u]) {
		int rec=dfs2(v);
		if(es.count({u,v})) z[es[{u,v}]]=rec; else
		if(es.count({v,u})) z[es[{v,u}]]=-rec; else
		assert(0);
		_z+=rec;
	}
	for(int v:adj[u]) {
		if(es.count({u,v})) {
			int i=es[{u,v}];
			if(!istre[i]) {
				assert(d[u]!=d[v]);
				if(d[v]>d[u]) --_z;
				if(d[v]<d[u]) ++_z;
			}
		}
		if(es.count({v,u})) {
			int i=es[{v,u}];
			if(!istre[i]) {
				assert(d[u]!=d[v]);
				if(d[v]>d[u]) --_z;
				if(d[v]<d[u]) ++_z;
			}
		}
	}
	return _z;
}





void _m(int tn) {
	int n,m; scanf("%d%d",&n,&m);
	for(int i=1;i<=n;i++) adj[i].clear(), seen[i]=0, ch[i].clear();
	es.clear();
	for(int i=1;i<=m;i++) {
		int u,v; scanf("%d%d",&u,&v);
		uu[i]=u, vv[i]=v;
		adj[u].pb(v);
		adj[v].pb(u); // direction either way, use es
		es[{u,v}]=i;
		istre[i]=0;
		z[i]=0;
	}
	for(int i=1;i<=n;i++) {
		sort(adj[i].begin(),adj[i].end());
		adj[i].resize(unique(adj[i].begin(),adj[i].end())-adj[i].begin());
	}

	int rt=0;
	for(int i=1;i<=n;i++) if(adj[i].size()) rt=i;
	assert(rt);
	dfs1(rt);
	for(int i=1;i<=n;i++) if(!seen[i] && adj[i].size()) goto fail;
		// cerr<<"survided seen check"<<endl;

	assert(dfs2(rt)==0);
	for(int i=1;i<=m;i++) if(!istre[i]) {
		z[i]=1;
		if(d[uu[i]] < d[vv[i]]) z[i]=-1;
	}

	// cerr<<"gonna z[i] check:";
	// for(int i=1;i<=m;i++) cerr<<" "<<z[i];
	// cerr<<endl;

	for(int i=1;i<=m;i++) if(!z[i]) goto fail;
		// cerr<<"survided nz z[i] check"<<endl;

	printf("Case #%d:",tn);
	for(int i=1;i<=m;i++) printf(" %d",z[i]);
	printf("\n");

	return;
	fail:;
	printf("Case #%d: IMPOSSIBLE\n",tn);
}

int32_t main() {
	int T; scanf("%d",&T); for(int tn=1;tn<=T;tn++) _m(tn);
}










