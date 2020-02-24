// 23:13 - 23:49
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
using namespace std;

#define forint(i, a, b) for(int i=int(a); i<=int(b); ++i)
#define forintdown(i, a, b) for(int i=int(a); i>=int(b); --i)
#define forn(i, n) forint(i, 0, (n)-1)
#define forndown(i, n) forintdown(i, (n)-1, 0)
#define fillchar(a, x) memset(a, x, sizeof(a))
#define fi first
#define se second
#define PB push_back
#define MP make_pair
typedef long long LL;
typedef pair<double,double> PDD;
typedef pair<int,int> PII;
typedef pair<LL,LL> PLL;
typedef vector<int> VI;
typedef vector<LL> VL;
typedef vector<PII> VPI;
typedef vector<PLL> VPL;

const int MAXN=4010;
const int MAXE=MAXN*3;

int g[MAXN], g_src[MAXE], g_lnk[MAXE], g_nxt[MAXE];
int ex[MAXN];
int coins[MAXN];
int n, en;

void init() {
	scanf("%d", &n);
	en=0; fillchar(g, 0);
	forint(i, 1, n)
		scanf("%d", &coins[i]);
	forint(i, 1, n-1) {
		int a=i, b; scanf("%d", &b);
		int e;
		e=++en, g_src[e]=a, g_lnk[e]=b, g_nxt[e]=g[a], g[a]=e;
		e=++en, g_src[e]=b, g_lnk[e]=a, g_nxt[e]=g[b], g[b]=e;
	}
	forint(i, 1, n) {
		int e=++en;
		ex[i]=e;
		g_src[e]=0, g_lnk[e]=i;
	}
}

int dst[MAXN][MAXN], dfi[MAXN][MAXN];
VPI lx[MAXN];

void bfs(int start) {
	static int que[MAXN];
	static bool vis[MAXN];
	fillchar(vis, false);
	int ss=0, tt=0;
	vis[start]=true;
	for(int e=g[start]; e; e=g_nxt[e]) {
		int x=g_lnk[e];
		vis[x]=true;
		que[++tt]=x;
		dst[start][x]=coins[x];
		dfi[start][x]=x;
	}
	while(ss<tt) {
		int x=que[++ss];
		for(int e=g[x]; e; e=g_nxt[e]) {
			int y=g_lnk[e];
			if(vis[y]) continue;
			vis[y]=true;
			que[++tt]=y;
			dst[start][y]=dst[start][x]+coins[y];
			dfi[start][y]=dfi[start][x];
		}
	}
	lx[start].clear();
	forint(x, 1, n)
		if(x!=start)
			lx[start].PB(MP(dst[start][x], dfi[start][x]));
	sort(lx[start].rbegin(), lx[start].rend());
	VPI p;
	forn(i, lx[start].size())
		if(p.size()==0 || lx[start][i].se!=p.back().se)
			p.PB(lx[start][i]);
	lx[start]=p;
}

int lon[MAXE];

void precount() {
	forint(i, 1, n) bfs(i);
	forint(e, 1, en) {
		int x=g_lnk[e], px=g_src[e];
		lon[e]=coins[x];
		forint(y, 1, n) if(y!=x)
			if(dfi[x][y]!=px)
				lon[e]=max(lon[e], coins[x]+dst[x][y]);
	}
}

int f[MAXE][MAXE];
bool bf[MAXE][MAXE];

int cal_f_equalcase(int x, int px, int py) {
	int pz=0;
	int tmp1=0;
	forn(i, min(4u,lx[x].size())) {
		int p=lx[x][i].se;
		if(p==px || p==py) continue;
		int v=lx[x][i].fi;
		if(v>tmp1)
			tmp1=v, pz=p;
	}
	int tmp2=0;
	forn(i, min(4u,lx[x].size())) {
		int p=lx[x][i].se;
		if(p==px || p==py || p==pz) continue;
		int v=lx[x][i].fi;
		if(v>tmp2)
			tmp2=v;
	}
	return tmp1-tmp2;
}

int t[MAXE][MAXN];
bool bt[MAXE][MAXN];

int cal_t(int e, int x) {
	int &ans=t[e][x];
	if(bt[e][x]) return ans;
	bt[e][x]=true;

	int py=g_src[e], y=g_lnk[e];
	ans=0;
	for(int i=g[y]; i; i=g_nxt[i]) {
		int z=g_lnk[i];
		if(z==x || z==py) continue;
		if(dfi[y][x]==z)
			ans=max(ans, coins[z]+cal_t(i, x));
		else
			ans=max(ans, lon[i]);
	}
	return ans;
}

int cal(int p1, int p2, int x, int py, int y, int ey) {
	int ans;
	ans=cal_t(ey, x);

	int tmp=0;
	int p3=dfi[x][y];
	for(size_t i=0; i<min(lx[x].size(), 4u); ++i) {
		int p=lx[x][i].se;
		if(p==p1 || p==p2 || p==p3) continue;
		tmp=max(tmp, lx[x][i].fi);
	}
	tmp+=dst[y][x]-coins[x];
	return max(ans, tmp);
}

int cal_f(int e1, int e2) {
	int &ans=f[e1][e2];
	if(bf[e1][e2]) return ans;
	bf[e1][e2]=true;

	int px=g_src[e1], x=g_lnk[e1];
	int py=g_src[e2], y=g_lnk[e2];
	if(x==y)
		return ans=cal_f_equalcase(x, px, py);

	ans=-(1<<30);
	for(int e=g[x]; e; e=g_nxt[e]) {
		int z=g_lnk[e];
		if(z==px) continue;
		int tmp;

		if(z==dfi[x][y]) {
			int bonus=(z==y ? 0 : coins[z]);
			tmp=bonus-cal_f(e2, e);
		} else {
			tmp=lon[e];
			tmp-=cal(px, z, x, py, y, e2);
		}
		ans=max(ans, tmp);
	}

	return ans;
}

void solve() {
	init();
	precount();

	fillchar(bf, false);
	fillchar(bt, false);

	int ans=-(1<<30);
	forint(x, 1, n) {
		int tmp=1<<30;
		forint(y, 1, n) {
			int cur=cal_f(ex[x], ex[y]);
			cur+=coins[x];
			if(x!=y) cur-=coins[y];
			tmp=min(tmp, cur);
		}
		ans=max(ans, tmp);
	}
	printf("%d\n", ans);
}

int main() {
	int ncs; scanf("%d", &ncs);
	forint(cs, 1, ncs) {
		cerr<<cs<<endl;
		printf("Case #%d: ", cs);
		solve();
	}
	return 0;
}
