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

#define FN "C-small-attempt0"

#define N 128
#define MX 1000
int n;
int x[N],y[N];
char val[N];

int main()
{
	freopen(FN ".in","r",stdin);
	freopen(FN ".out","w",stdout);

	int tests;
	scanf("%d",&tests);
	for (int test = 1; test<=tests; test++)
	{
		scanf("%d",&n);
		REP(i,n)
		{
			scanf("%d%d",&x[i],&y[i]);
			do val[i] = getc(stdin); while (val[i]!='#'&&val[i]!='.');
		}
		bool hasres = false;
		int rx=-1,ry=-1;
		FOR(xx,-MX,MX) FOR(yy,-MX,MX)
		{
			REP(i,n)
				if ((max(abs(xx-x[i]),abs(yy-y[i]))%2 ? '#' : '.') != val[i])
					goto bad;
			if (!hasres || abs(xx)+abs(yy) < abs(rx)+abs(ry) || abs(xx)+abs(yy) == abs(rx)+abs(ry) && PII(xx,yy) > PII(rx,ry))
			{
				rx = xx;
				ry = yy;
				hasres = true;
			}
			bad:;
		}
		printf("Case #%d: ",test);
		if (hasres)
			printf("%d %d\n",rx,ry);
		else
			printf("Too damaged\n");
	}
	return 0;
}