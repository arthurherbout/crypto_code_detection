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






tuple<int,int,int> adj[16][2];
int seen[16];


void _m(int tn) {
	int c; scanf("%d",&c);
	for(int i=1;i<=c;i++) {
		int e,l,d;
		scanf("%d%d%d",&e,&l,&d);
		adj[i][0]={e,l,d};
		scanf("%d%d%d",&e,&l,&d);
		adj[i][1]={e,l,d};
	}

	int bes=inf;
	for(int mk=1<<c;--mk>=0;) {
		for(int i=1;i<=c;i++) seen[i]=0;
		int T=0;
		int u=1;
		for(;u!=1 || seen[1]<2;) {
			int i=seen[u];
			assert(i==0 || i==1);
			seen[u]++;
			if((mk<<1) & (1<<u)) i^=1;
			for(;T%24 != get<1>(adj[u][i]);) ++T;
			T+=get<2>(adj[u][i]);
			u=get<0>(adj[u][i]);
		}
		for(int i=1;i<=c;i++) if(seen[i]!=2) goto skip;
		bes=min(bes,T);
		skip:;
	}
	printf("Case #%d: %d\n",tn,bes);
}

int32_t main() {
	int T; scanf("%d",&T); for(int tn=1;tn<=T;tn++) _m(tn);
}










