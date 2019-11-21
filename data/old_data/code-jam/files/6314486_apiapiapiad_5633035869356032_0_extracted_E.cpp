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




int _,__;
int n,c,id,p,x,v,s;
vector<PII> st[60100];
set<vector<vector<PII> > > hs;
bool check(vector<vector<PII> > st) {
	bool fin=1;
	for (auto p:st) {
		if (SZ(p)>1) { fin=0; break; }
	}
	if (fin) return 1;
	return 0;
}
bool dfs(vector<vector<PII> > st) {
	if (check(st)) return 1;
	if (hs.count(st)) return 0;
	int m=SZ(st);
	hs.insert(st);
	while (1) {
		bool rd=0;
		rep(i,0,m) rep(j,0,m) if (i!=j&&!st[i].empty()&&!st[j].empty()) {
			if (st[i][0].se==st[j][0].se) {
				if (st[i][0].fi<st[j][0].fi) st[i].erase(st[i].begin());
				else st[j].erase(st[j].begin());
				rd=1;
			}
		}
		if (rd==0) break;
	}
	if (check(st)) return 1;
	rep(i,0,m) rep(j,0,m) if (st[i].empty()&&!st[j].empty()) {
		vector<vector<PII> > nst=st;
		nst[i].pb(st[j][0]);
		nst[j].erase(nst[j].begin());
		if (dfs(nst)) return 1;
	}
	return 0;
}
bool solve() {
	hs.clear();
	scanf("%d%d",&n,&c);
	vector<vector<PII> > sta;
	rep(i,0,n) {
		scanf("%d",&id);
		sta.pb(st[id]);
	}
	return dfs(sta);
}
int main() {
	scanf("%d",&p);
	rep(i,0,p) {
		scanf("%d",&x);
		rep(j,0,x) {
			scanf("%d%d",&v,&s);
			st[i].pb(mp(v,s));
		}
	}
	for (scanf("%d",&_);_;_--) {
		printf("Case #%d: ",++__);
		fprintf(stderr,"Case #%d\n",__);
		puts(solve()?"POSSIBLE":"IMPOSSIBLE");
	}
}
 
