#include <cstdio>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
#include <stack>
#include <list>
#include <queue>
#include <deque>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <numeric>
#include <cmath>
#include <cstring>
using namespace std;

typedef vector <int > VI;
typedef vector < VI > VVI;
typedef long long LL;
typedef vector < LL > VLL;
typedef long double LD;
typedef vector < LD > VD;
typedef vector < string > VS;
typedef pair<int,int> PII;
typedef vector <PII> VPII;
typedef istringstream ISS;

#define ALL(x) x.begin(),x.end()
#define REP(i,n) for (int i=0; i<(n); ++i)
#define FOR(var,pocz,koniec) for (int var=(pocz); var<=(koniec); ++var)
#define FORD(var,pocz,koniec) for (int var=(pocz); var>=(koniec); --var)
#define FOREACH(it, X) for(__typeof((X).begin()) it = (X).begin(); it != (X).end(); ++it)
#define PB push_back
#define PF push_front
#define MP(a,b) make_pair(a,b)
#define ST first
#define ND second
#define SIZE(x) (int)x.size()

#define POINTT LD // Dla wspolrzednych punktu (int lub double)
#define POINTR LD // Dla wynikow operacji - pole, iloczyn wektorowy (LL lub double)
#define EPS 1E-9
struct POINT {
	POINTT x,y;
	POINT(POINTT wx, POINTT wy) : x(wx), y(wy) {}
	POINT() {}
};

inline bool IsZero(LD x){
	return x > -EPS && x < EPS;
}

#define SQ(x) ((x)*(x))
vector<POINT> LineCircleCross(POINT p, POINTR cr, POINT p1, POINT p2){
	LD a = SQ(p1.x) + SQ(p1.y) +SQ(p2.x) + SQ(p2.y) - 2.0 * (p1.x * p2.x + p1.y * p2.y);
	LD b = 2.0*(p.x*(p2.x-p1.x)+p.y*(p2.y-p1.y)+p1.x*p2.x+p1.y*p2.y-SQ(p2.x)-SQ(p2.y));
	LD c = -SQ(cr)+SQ(p2.x)+SQ(p2.y)+SQ(p.x)+SQ(p.y)-2.0*(p.x*p2.x+p.y*p2.y);
	LD d = b*b-4.0*a*c;
	vector<POINT> r;
	if (d < -EPS) return r;
	LD t = -b/(2.0*a);
	if (IsZero(d))
		r.PB(POINT(t*p1.x +(1.0-t)*p2.x, 0.0));
	else{
		LD e = sqrt(d)/(2.0*a);
		r.PB(POINT((t+e)*p1.x+(1.0-t-e)*p2.x, 0.0));
		r.PB(POINT((t-e)*p1.x+(1.0-t+e)*p2.x, 0.0));
	}
	return r;
}


const int N = 550;
pair<POINT,POINT> styczne[2][N];
int n;
int xred, yred, xgreen, ygreen;
int x[N], y[N], r[N];

inline void dodaj(vector<LD> &kand, LD yy){
	if (yy > 0 && yy < 100.0) kand.PB(yy);
}

inline bool between(LD a, LD b, LD c){
	return a >= min(b,c) && a <= max(b,c);
}

bool czy_dochodzi(LD x0, LD y0, LD x1, LD y1){
	REP(i,n){
		vector<POINT> p = LineCircleCross(POINT(x[i],y[i]), r[i], POINT(x0,y0), POINT(x1,y1));
		FOREACH(it,p) if (between(it->x, x0, x1)) return 0;
	}
	return 1;
}

inline LD sqr(LD x){return x*x;}

vector<LD> licz_w_punkcie(LD xx){
	vector<LD> kand;
	kand.PB(0);
	kand.PB(100.0);
	REP(i,n) if (x[i]-r[i] < xx && xx < x[i]+r[i]){
		LD foo = sqrtl(sqr(r[i])-sqr(x[i]-xx));
		dodaj(kand,y[i]+foo);
		dodaj(kand,y[i]-foo);
	}
	REP(i,n) REP(j,2){
		LD ykolor = j == 0 ? yred : ygreen;
		LD xkolor = j == 0 ? xred : xgreen;

		if (!IsZero(styczne[j][i].ST.x-xx)){
			LD y0 = ykolor;
			LD y1 = styczne[j][i].ST.y;
			LD dist = styczne[j][i].ST.x - xkolor;
			LD foo = y0 + (y1-y0) * (xx - xkolor) / dist;
			dodaj(kand,foo);
		}
		if (!IsZero(styczne[j][i].ND.x-xx)){
			LD y0 = ykolor;
			LD y1 = styczne[j][i].ND.y;
			LD dist = styczne[j][i].ND.x - xkolor;
			LD foo = y0 + (y1-y0) * (xx - xkolor) / dist;
			dodaj(kand,foo);
		}
	}
	sort(ALL(kand));
//	FOREACH(it, kand) printf("yy=%.6Lf\n", *it);
	vector<LD> res(4, 0.0);
	REP(i,SIZE(kand)-1){
		LD y0 = (kand[i+1] + kand[i])/2.0;
		int jest_red = czy_dochodzi(xred,yred,xx,y0);
		int jest_green = czy_dochodzi(xgreen,ygreen,xx,y0);
		res[jest_red + 2*jest_green] += kand[i+1] - kand[i];
	}
//	REP(i,4) printf("res[%d] = %.6Lf\n",i,res[i]);
	return res;
}

LD res[4]; //(black, red, green, both)
vector<LD> ostatni;

void licz(LD l, LD r){
	LD d = (r-l)/2;
	vector<LD> f1;
  if (!ostatni.empty()){
		f1 = ostatni;
	} else {
		f1 = licz_w_punkcie(l);
	}
	vector<LD> f2 = licz_w_punkcie(l+d);
	vector<LD> f3 = licz_w_punkcie(r);
	ostatni = f3;
	REP(i,4){
		res[i] += (r - l) * (f1[i] + 4.0 * f2[i] + f3[i]) / 6.0;
	}
}

pair<POINT,POINT> licz_styczne(LD x0, LD y0, LD x1, LD y1, LD r1){
	LD d = hypotl(x1-x0, y1-y0);
	LD alfa_zero = atan2l((y1 - y0), (x1-x0));
	LD alfa = asinl(r1/d);
	pair<POINT,POINT> resp;
	REP(i,2){
		LD kat = alfa_zero;
		if (i == 0) kat  += alfa;
		else kat -= alfa;
		LD dd = sqrtl(sqr(d)-sqr(r1));
		POINT p = POINT(x0 + dd * cosl(kat), y0 + dd * sinl(kat));
		if (i==0) resp.ST = p;
		else resp.ND = p;
	}
	return resp;
}

void wypisz(POINT p){
	printf("punkt (%.6Lf,%.6Lf)\n", p.x, p.y);
}

int main(){
  int testy;
  scanf("%d",&testy);
  REP(numer_testu,testy){
    printf("Case #%d:\n", numer_testu+1);
		scanf("%d %d",&xred,&yred);
		scanf("%d %d",&xgreen,&ygreen);
		scanf("%d",&n);
		REP(i,n) scanf("%d %d %d",x+i,y+i,r+i);

		REP(i,n){
		 	styczne[0][i] = licz_styczne(xred,yred,x[i],y[i],r[i]);
		 	styczne[1][i] = licz_styczne(xgreen,ygreen,x[i],y[i],r[i]);
	//		REP(j,2) wypisz(styczne[j][i].ST),wypisz(styczne[j][i].ND);
	//		POINT p = styczne[0][i].ST;
		//	printf("dist = %.6Lf\n", sqrtl(sqr(r[i])+sqr(p.x-xred)+sqr(p.y-yred)));
		}
		/*
		VD kand;
		kand.PB(0);
		kand.PB(100);
		kand.PB(xred);
		kand.PB(xgreen);
		REP(i,n){
		 	styczne[0][i] = licz_styczne(xred,yred,x[i],y[i],r[i]);
		 	styczne[1][i] = licz_styczne(xgreen,ygreen,x[i],y[i],r[i]);
			REP(j,2){
				kand.PB(styczne[j][i].ST.x);
				kand.PB(styczne[j][i].ND.x);
			}
		}
		sort(ALL(kand));
		*/
		const int KROKI = 900000;
		LD krok = 100.0 / KROKI;
		REP(i,4) res[i] = 0.0;
//		licz_w_punkcie(50.0);
//	  return 0;
		REP(foo,KROKI){
			LD ll = foo * krok;
			LD rr = ll + krok;
			ostatni.clear();
			licz(ll,rr);
		}
		LD PI = 2.0 * acosl(0.0);
//		printf("pi = %Lf\n", PI);
		REP(i,n) res[0] -= PI * r[i] * r[i];
		REP(i,4) printf("%.9Lf\n", (long double)res[i]);
		fflush(stdout);
  }
  return 0;
}
