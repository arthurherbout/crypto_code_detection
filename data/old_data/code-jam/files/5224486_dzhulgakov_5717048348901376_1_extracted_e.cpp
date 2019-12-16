#define _CRT_SECURE_NO_WARNINGS
#pragma comment(linker,"/STACK:32000000")
#include <algorithm>
#include <numeric>
#include <string>
#include <list>
#include <cstring>
#include <set>
#include <map>
#include <vector>
#include <queue>
#include <iostream>
#include <iterator>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cassert>
#include <unordered_map>

using namespace std;

#define REP(i,n) for (int i=0,_n=(n); i < _n; i++)
#define REPD(i,n) for (int i=(n)-1; i >= 0; i--)
#define FOR(i,a,b) for (int _b=(b), i=(a); i <= _b; i++)
#define FORD(i,a,b) for(int i=(a),_b=(b);i>=_b;i--)
#define ALL(c) (c).begin(), (c).end()
#define SORT(c) sort(ALL(c))

#define CLEAR(x) memset(x,0,sizeof x);
#define CLEARA(x) memset(&x,0,sizeof x);
#define FILL(x,v) memset(x,v,sizeof x);
#define FILLA(x,v) memset(&x,v,sizeof x);

#define VAR(a,b) __typeof(b) a=(b)
#define FOREACH(it,c) for(VAR(it,(c).begin());it!=(c).end();++it)

#define REVERSE(c) reverse(ALL(c))
#define UNIQUE(c) SORT(c),(c).resize(unique(ALL(c))-(c).begin())
#define INF 0x7fffffff
#define X first
#define Y second
#define pb push_back
#define SZ(c) (int)(c).size()
#define MP make_pair
#define eps 1.0e-11
const double pi = acos(-1.0);

typedef pair<int, int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long LL;

#define N 128

int n,m;
int a[N][N];
map<int,VI> cl[2];

void apply(VI&st,int xx)
{
	REP(j,m)
	{
		st[j] = max(0,st[j]+a[xx][j]);
	}
}

int total(const VI& st) {
	int s = 0;
	REP(i,m) s+= st[i];
	return s;
}

int getsign(const VI&st)
{
	int mask = 0;
	REP(j,m) if (st[j] > 0) mask |= 1<<j;
	return mask;
}

VVI force(const VI& init, map<int,VI>& cl)
{
	VI order;
	for (const auto& it : cl) order.push_back(it.first);
	UNIQUE(order);
	SORT(order);

	vector<VVI> best(1<<m,VVI(1<<m,VI(m,-1)));
	best[0][getsign(init)] = init;

	REP(mask,1<<m) REP(bit,m) if (!(mask&(1<<bit))) REP(sss,1<<m) if (best[mask][sss][0] != -1)
	{
		int newmask = mask|(1<<bit);
		VI st = best[mask][sss];
		for (const auto& lst : cl)
		{
			if ((lst.first & mask) != lst.first && (lst.first & newmask) == lst.first)
				for (const auto& ind : lst.second)
					apply(st,ind);
		}
		int news = getsign(st);
		if (total(st) > total(best[newmask][news]))
			best[newmask][news] = st;
	}
	return best[(1<<m)-1];
}

VI force2(const VI& init,map<int,VI>& cl)
{
	VI order;
	for (const auto& it : cl) order.push_back(it.first);
	UNIQUE(order);
	SORT(order);

	VVI best(1<<m,VI(m,-1));
	best[0] = init;

	REP(mask,1<<m) REP(bit,m) if (!(mask&(1<<bit)))
	{
		int newmask = mask|(1<<bit);
		VI st = best[mask];
		for (const auto& lst : cl)
		{
			if ((lst.first & mask) != lst.first && (lst.first & newmask) == lst.first)
				for (const auto& ind : lst.second)
					apply(st,ind);
		}
		if (total(st) > total(best[newmask]))
			best[newmask] = st;
	}
	return best[(1<<m)-1];
}

int main(int argc, char **argv)
{
	string FN = "E-large";
	if (argc>1) FN = string(argv[1]);
	int shift = 0;
	if (argc>2) sscanf(argv[2],"%d",&shift);
	freopen((FN+".in").c_str(),"r",stdin);
	freopen((FN+".out").c_str(),"w",stdout);

	int tests;
	scanf("%d",&tests);
	for (int test = 1; test<=tests; test++)
	{
		fprintf(stderr,"=== %s : %d\n", FN.c_str(), test+shift);
		printf("Case #%d: ",test+shift);
		////////////////////////////////////////////////////////////
		scanf("%d%d",&n,&m);
		fprintf(stderr,"%d====\n",m);
		REP(i,n) REP(j,m) scanf("%d",&a[i][j]);
		REP(i,2)cl[i].clear();
		REP(i,n)
		{
			int mask = 0;
			REP(j,m) if (a[i][j] >= 0) mask |= 1<<j;
			int s = 0;
			REP(j,m) s+=a[i][j];
			cl[s>=0][mask].pb(i);
		}
		auto q = force(VI(m,0),cl[0]);
		int res = 0;
		int cnt = 0;
		REP(iii,SZ(q))
		{
			const auto& it = q[iii];
			if (total(it) < 0) continue;
			++cnt;
			auto t = force2(it,cl[1]);
			res=max(res,total(t));
		}
		fprintf(stderr,"? %d\n",cnt);
		printf("%d\n",res);
	}
	fprintf(stderr,"=== %s DONE\n", FN.c_str());
	return 0;
}