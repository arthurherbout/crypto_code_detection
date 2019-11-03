#include <string>
#include <cctype>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <cstdlib>
#include <cstdio>
#include <queue>
#include <stack>
#include <memory.h>
#include <assert.h>
using namespace std;
#define SZ(a) (int)(a).size()
#define FOR(i,a,b) for (int i=(a); i<=(b); ++i)
#define REP(i,n) for (int i=0; i<(n); ++i)
#define ALL(c) c.begin(), c.end()
#define CLR(c,n) memset(c,n,sizeof(c))
#define TR(container, it) for(typeof(container.begin()) it = container.begin(); it != container.end(); it++)
#define CONTAIN(container, it) (container.find(it)!=container.end())
#define MCPY(dest,src) memcpy(dest,src,sizeof(src))
typedef long long LL;
typedef vector<int> VI;
typedef vector<string> VS;
typedef pair<int, int> PII;
const double EPS=1e-12;
const double PI=acos(-1);
const int INF=0x3F3F3F3F;
typedef pair<double,double> PT;
#define X first
#define Y second
int n, m;
PT p[5000], q[1000], c;
double r[5000];
bool check(double x, double y) {
	REP(i,n) if (hypot(p[i].X-x,p[i].Y-y)>r[i]) return false;
	return true;
}
double check(double xmin, double xmax, double ymin, double ymax) {
	double dx=xmax-xmin, dy=ymax-ymin, area=dx*dy;
	double xmid=(xmax+xmin)/2, ymid=(ymax+ymin)/2;
	bool c1=check(xmin,ymin), c2=check(xmin,ymax), c3=check(xmax,ymin), c4=check(xmax,ymax);
	if (c1&&c2&&c3&&c4) return area;
	else if (!c1&&!c2&&!c3&&!c4) return area;
	else if (area<EPS) return area/2;
	else if (dx>dy) return check(xmin,xmid,ymin,ymax)+check(xmid,xmax,ymin,ymax);
	else return check(xmin,xmax,ymin,ymid)+check(xmin,xmax,ymid,ymax);
}
double check(PT mid) {
	REP(i,n) r[i]=hypot(p[i].X-mid.X, p[i].Y-mid.Y);
	double xmin=mid.X, xmax=mid.X, ymin=mid.Y, ymax=mid.Y;
	REP(i,n) {
		xmin<?=p[i].X-r[i]; xmax>?=p[i].X+r[i];
		ymin<?=p[i].X-r[i]; ymax>?=p[i].Y+r[i];
	}
	c=mid;
	return check(xmin, xmax, ymin, ymax);
}
int main(int argc, char *argv[])
{
	freopen("D-large.in", "r", stdin);
	freopen("D.out", "w", stdout);
	int test_case;
	scanf("%d", &test_case);
	for (int test_case_id=1; test_case_id<=test_case; ++test_case_id) {
		cin>>n>>m;
		REP(i,n) cin>>p[i].X>>p[i].Y; REP(i,m) cin>>q[i].X>>q[i].Y;
		fprintf(stderr, "Case %d of %d\n", test_case_id, test_case);
		printf("Case #%d:", test_case_id);
		REP(i,m) {
			printf(" %.7lf", check(q[i]));
		}
		printf("\n");
	}
}

