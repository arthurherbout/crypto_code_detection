#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>
using namespace std;

#define FOR(i,a,b)  for(int i=(a);i<(b);++i)
#define REP(i,a)    FOR(i,0,a)
#define FORE(i,a)   for(__typeof(a.begin()) i=a.begin();i!=a.end();i++)
#define ZERO(m)     memset(m,0,sizeof(m))
#define ALL(x)      x.begin(),x.end()
#define PB          push_back
#define S           size()
#define LL          long long
#define LD          long double
#define MP          make_pair
#define PII         pair<int, int>
#define X           first
#define Y           second
#define VC          vector
#define VI          VC<int>
#define VVI         VC< VI >
#define VS          VC<string>
#define VPII        VC< PII >
#define DB(a)       cerr << #a << ": " << a << endl

void print(VI v) {cout << "[";if (v.S) cout << v[0];FOR(i, 1, v.S) cout << ", " << v[i];cout << "]\n";}
void print(VS v) {cout << "[";if (v.S) cout << v[0];FOR(i, 1, v.S) cout << ", " << v[i];cout << "]\n";}
void print(VVI v) {cout << "[ ---";if (v.S) cout << " ", print(v[0]);FOR(i, 1, v.S) cout << " ", print(v[i]);	cout << "--- ]\n";}
void print(PII p) {cout << "{" << p.X << ", " << p.Y << "}";}
void print(VPII v) {cout << "[";if (v.S) print(v[0]);FOR(i, 1, v.S)  cout << ", ", print(v[i]);cout << "]\n";}

template<class T> string i2s(T x) {ostringstream o; o << x; return o.str(); }
VS splt(string s, char c = ' ') {VS rv; int p = 0, np; while (np = s.find(c, p), np >= 0) {if (np != p) rv.PB(s.substr(p, np - p)); p = np + 1;} if (p < s.S) rv.PB(s.substr(p)); return rv;}


double dist(PII p1, PII p2) {
	return sqrt((p1.X - p2.X) * (p1.X - p2.X) + (p1.Y - p2.Y) * (p1.Y - p2.Y));
}

double dist(double p1X, double p1Y, double p2X, double p2Y) {
	return sqrt((p1X - p2X) * (p1X - p2X) + (p1Y - p2Y) * (p1Y - p2Y));
}
VI pr;
		VPII t1, t2;
		
double test(double v0, double v1, double v2) {
	if (v0 < 0 || v1 < 0 || v2 < 0 || v1 > 1 || v0 > 1 || v2 > 1) return 1e20;
	double norm = v0 + v1 + v2;
	double n0 = v0 / norm, n1 = v1 / norm, n2 = v2 / norm;
	return dist(t1[0].X * n0 + t1[1].X * n1 + t1[2].X * n2, t1[0].Y * n0 + t1[1].Y * n1 + t1[2].Y * n2,  
				t2[0].X * n0 + t2[1].X * n1 + t2[2].X * n2, t2[0].Y * n0 + t2[1].Y * n1 + t2[2].Y * n2);
	
}

int main() {
	int tc;
	cin >> tc;
	REP(atc, tc) {
		printf("Case #%d: ", atc + 1);
		t1.clear(), t2.clear();
		REP(i, 3) {
			int a,  b;
			cin >> a >> b;
			t1.PB(MP(a, b));
		}
		REP(i, 3) {
			int a,  b;
			cin >> a >> b;
			t2.PB(MP(a, b));
		}

		//print(pr);
		double v0 = 0.5, v1 = 0.5, v2 = 0.5;
		double av = test(v0, v1, v2);
		REP(i, 80000000){
			int x = rand() % 3;
			double df = (rand() % 65000000 - 32000000) / 10000000000.0;
			if (x == 0) v0 += df; else if (x == 1) v1 += df; else v2 += df;
			double nv = test(v0, v1, v2);
			if (nv < av) {
				av = nv;
				continue;
			}
			if (x == 0) v0 -= df; else if (x == 1) v1 -= df; else v2 -= df;
			
		}
		//(v0); DB(v1); DB(v2);
		//(av);
		double sum = v0 + v1 + v2;
		v0 /= sum; v1 /= sum; v2 /= sum;
		if (av > 1e-6) cout << "No Solution" << endl; else {
			cout << (t2[0].X * v0 + t2[1].X * v1 + t2[2].X * v2) << " " << (t2[0].Y * v0 + t2[1].Y * v1 + t2[2].Y * v2) << endl;
		}

	}
}
