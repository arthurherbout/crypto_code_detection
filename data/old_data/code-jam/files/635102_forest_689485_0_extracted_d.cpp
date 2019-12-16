#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <memory.h>
#include <queue>
#include <cmath>

using namespace std;

typedef vector<int> VI;
typedef pair<int, int> PI;
typedef vector<PI> VPI;

#define FOR(i,a,n) for (int i = (a); i < (n); ++i)
#define FORE(i,a,n) for (int i = (a); i <= (n); ++i)
#define FORD(i,a,b) for (int i = (a); i >= (b); --i)
#define REP(i,n) FOR(i,0,n)
#define REPE(i,n) FORE(i,0,n)
#define LL long long
#define FIR(n) REP(i,n)
#define FJR(n) REP(j,n)
#define ALL(v) v.begin(), v.end()

#define FI FIR(n)
#define FJ FJR(n)
#define FR(i,a) FOR(i,a,n)
#define REPN(i) REP(i,n)
#define GI(n) scanf("%d", &n)
#define GI2(n, m) scanf("%d %d", &n, &m)

long double d, r1, r2;
#define PI 3.141592653589793238462643383

double solve() {
	if (d >= r1+r2) return 0;
	if (r2>= d+r1) PI*r1*r1;

	//double a = (d*d+r1*r1-r2*r2) / 2/d;

	return r1*r1*acos( (d*d+r1*r1-r2*r2) / (2*d*r1))+
		    r2*r2*acos( (d*d-r1*r1+r2*r2) / (2*d*r2))-
			 0.5*sqrt( (-d+r1+r2)*(d-r1+r2)*(d+r1-r2)*(d+r1+r2) );

}

int main() {
freopen("D-small-attempt2.in", "rt", stdin);
freopen("D-small-attempt2.out", "w", stdout);
//	freopen("input.txt", "rt", stdin);


	int ncases;
   GI(ncases); 
	FORE(ncase, 1, ncases) {
		double x1, y1, x2, y2;
		int n, m;
		GI2(n, m);
		if (n!=2) cerr <<"ATTENTION!" << endl;

		scanf("%lf %lf %lf %lf", &x1, &y1, &x2, &y2);
		d = sqrt ( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

		printf("Case #%d:", ncase);

		FIR(m) {
			double x, y;
			scanf("%lf %lf", &x, &y);
			r1 = sqrt ( (x1-x)*(x1-x) + (y1-y)*(y1-y));
			r2 = sqrt ( (x-x2)*(x-x2) + (y-y2)*(y-y2));

			if (r1 > r2) swap(r1, r2);
			double res = solve();
			printf(" %.7lf", res);
		}

		printf("\n");

	}
}
