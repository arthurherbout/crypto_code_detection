#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

const double million = 1000*1000;

int m;
double p;
double x;

double go(int turns, double ratio)
{
	if(ratio <= 1.0) return 1.0;
	if(turns == 1)
	{
		if(ratio <= 2.0) return p;
		return 0;
	}
	if(pow(2.0, turns) < ratio) return 0.0;
	double lo = 0, hi = 1;
	double last = 0;
	while(hi-lo > 1e-7)
	{
		double pa = (lo*2+hi)/3;
		double pb = (lo+hi*2)/3;
		double sa = p*go(turns-1, ratio/(1.0+pa))+(1-p)*go(turns-1, ratio/(1.0-pa));
		double sb = p*go(turns-1, ratio/(1.0+pb))+(1-p)*go(turns-1, ratio/(1.0-pb));
		last = max(sa, sb);
		if(sa > sb)		
			hi = pb;
		else
			lo = pa;
	}
	double bet = (lo+hi)/2;
//	printf("turns %d, ratio %.8lf => %.8lf (bet %.4lf)\n", turns, ratio, last, bet);
	return last;
}

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		fprintf(stderr, "%d of %d ..\r", cc+1, cases);
		printf("Case #%d: ", cc+1);
		cin >> m >> p >> x;
		printf("%.10lf\n", go(m, million/x));
	}
}
