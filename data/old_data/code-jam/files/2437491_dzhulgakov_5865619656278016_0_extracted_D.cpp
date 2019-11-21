#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <numeric>
#include <string>
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

#define N 111000
#define D 4
int n,d,k;
int a[N][D];
int reslen,resx;
map<int,int> seen;

bool has(int i, int val)
{
	REP(j,d) if (a[i][j] == val) return true;
	return false;
}

void check(int st, int len)
{
	if (len > reslen || len==reslen && st < resx)
	{
		reslen=len;
		resx=st;
	}
}

bool hh[N];
int done[N];
int aa[N][4],nn,before[N],ind[N];

int main(int argc, char **argv)
{
	string FN = "D-small-attempt1";
	if (argc>1) FN = string(argv[1]);
	int shift = 0;
	if (argc>2) sscanf(argv[2],"%d",&shift);
	freopen((FN+".in").c_str(),"r",stdin);
	freopen((FN+".out").c_str(),"w",stdout);

	int tests;
	cin >> tests;
	for (int test = 1; test<=tests; test++)
	{
		fprintf(stderr,"=== %s : %d\n", FN.c_str(), test+shift);
		printf("Case #%d: ",test+shift);
		////////////////////////////////////////////////////////////
		scanf("%d%d%d",&n,&d,&k);
		assert(k==2);
		seen.clear();
		REP(i,n) REP(j,d)
		{
			scanf("%d",&a[i][j]);
			++seen[a[i][j]];
		}
		//if (n>1000) { printf("---\n"); continue; }
		reslen=0;
		fprintf(stderr,"READ\n");
		int limit = 1000;
		REP(st,n) {
		if(st%10000 == 0) fprintf(stderr,"%d\n",st);
		REP(ind,d)
		{
			int val1 = a[st][ind];
			int cur = 1;
			while (cur < limit && st+cur < n && has(st+cur, val1))
			{
				cur++;
			}
			if (st+cur == n)
			{
				check(st,cur);
				continue;
			}
			REP(ind2,d)
			{
				int val2 = a[st+cur][ind2];

				int pp = st+cur;
				while (pp-st < limit && pp < n && (has(pp, val1) || has(pp, val2)))
				{
					pp++;
				}
				check(st,pp-st);
			}
		}
		}
		for (const auto& it : seen) {
			if (it.second*k < limit) continue;
			int val1 = it.first;
			nn = 0;
			FILL(ind,-1);
			CLEAR(done);
			CLEAR(before);
			REP(i,n)
			{
				if (ind[nn] == -1)
					ind[nn] = i;
				if (has(i,val1))
				{
					++before[nn];
					continue;
				}
				REP(j,d) aa[nn][j] = a[i][j];
				++nn;
			}
			if (nn == 0) check(0,n);
			REP(st,nn) REP(j,d) if (!(done[st]&(1<<j)))
			{
				int val2 = aa[st][j];
				int sum = before[st]+1;
				int p = st+1;
				while (p < nn)
				{
					int qq = find(aa[p],aa[p]+d,val2)-aa[p];
					if (qq >= d) break;
					done[p] |= 1<<qq;
					sum += before[p]+1;
					++p;
				}
				sum += before[p];
				check(ind[st],sum);
			}
		}
		printf("%d %d\n",resx,resx+reslen-1);
	}
	fprintf(stderr,"=== %s DONE\n", FN.c_str());
	return 0;
}