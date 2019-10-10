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
const double EPS=1e-9;
const double PI=acos(-1);
const int INF=0x3F3F3F3F;
typedef pair<double,double> PT;
#define X first
#define Y second
int n, m;
PT p[2], q[10];
double area(double r, double a) {
	return r*r*a-r*r*sin(a*2)/2;
}
int main(int argc, char *argv[])
{
	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D.out", "w", stdout);
	int test_case;
	scanf("%d", &test_case);
	for (int test_case_id=1; test_case_id<=test_case; ++test_case_id) {
		cin>>n>>m;
		REP(i,n) cin>>p[i].X>>p[i].Y; REP(i,m) cin>>q[i].X>>q[i].Y;
		fprintf(stderr, "Case %d of %d\n", test_case_id, test_case);
		printf("Case #%d:", test_case_id);
		double r=hypot(p[0].X-p[1].X,p[0].Y-p[1].Y);
		REP(i,m) {
			double r1=hypot(p[0].X-q[i].X,p[0].Y-q[i].Y), r2=hypot(p[1].X-q[i].X,p[1].Y-q[i].Y);
			double a1=acos((r1*r1+r*r-r2*r2)/2/r1/r),a2=acos((r2*r2+r*r-r1*r1)/2/r2/r);
			printf(" %.7lf", area(r1,a1)+area(r2,a2));
		}
		printf("\n");
	}
}

