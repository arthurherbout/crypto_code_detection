#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <ext/hash_set>
#include <ext/hash_map>
#include <ext/numeric>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

// begin_code_block \WFOR\(
#define FOR(i,a,b) for(int i=(a);i<(b);i++)
// end_code_block
// begin_code_block \WREP\(
#define REP(i,n) for(int i=0;i<(n);i++)
// end_code_block
// begin_code_block \WFORE\(
#define FORE(i,a,b) for(int i=(a);i<=(b);i++)
// end_code_block
// begin_code_block \WREPE\(
#define REPE(i,n) for(int i=0;i<=(n);i++)
// end_code_block
// begin_code_block \WSIZE\(
#define SIZE(c) ((int)(c).size())
// end_code_block
// begin_code_block \WFOREACH\(
#define FOREACH(i,c) for(typeof((c).begin()) i=(c).begin();i!=(c).end();++i)
// end_code_block
// begin_code_block \WALL\(
#define ALL(c) (c).begin(),(c).end()
// end_code_block
// begin_code_block \WIN\(
#define IN(a,x,b) ((a)<=(x)&&(x)<=(b))
// end_code_block
// begin_code_block \WUNIQUE\(
#define UNIQUE(v) (v).erase(unique((v).begin(),(v).end()),(v).end())
// end_code_block
// begin_code_block \WFILL\(
#define FILL(c,v) memset(c,v,sizeof(c))
// end_code_block
// begin_code_block \WCONTAINS\(
#define CONTAINS(s,x) (((s)&(1<<x))!=0)
// end_code_block
// begin_code_block \WADD\(
#define ADD(s,x) ((s)|(1<<x))
// end_code_block
// begin_code_block \WERASE\(
#define ERASE(s,x) ((s)&~(1<<x))
// end_code_block
// begin_code_block \WFORALL\(
#define FORALL(i,a,b,c) ({ bool _ok=true; for(int i=(a);i<(b);i++){ _ok = _ok && (c); if( !_ok ) break; } _ok; })
// end_code_block
// begin_code_block \WEXISTS\(
#define EXISTS(i,a,b,c) (!(FORALL(i,a,b,!(c))))
// end_code_block
// begin_code_block \WLL\W
typedef long long LL;
// end_code_block
// begin_code_block \WPII\W
typedef pair<int,int> PII;
// end_code_block
// begin_code_block \WVS\W
typedef vector<string> VS;
// end_code_block
// begin_code_block \WVI\W
typedef vector<int> VI;
// end_code_block
// begin_code_block \WVVI\W
typedef vector<VI> VVI;
// end_code_block
// begin_code_block \WVB\W
typedef vector<bool> VB;
// end_code_block
// begin_code_block \WVVB\W
typedef vector<VB> VVB;
// end_code_block
// begin_code_block \WVLL\W
typedef vector<LL> VLL;
// end_code_block
// begin_code_block \WVVLL\W
typedef vector<VLL> VVLL;
// end_code_block
// begin_code_block \WVPII\W
typedef vector<PII> VPII;
// end_code_block

// begin_code_block \Wsplit\(
vector<string> split(string s,string d){
	s += d[0];
	vector<string> ret;
	string w;
	FOREACH(it,s){
		if( d.find(*it) == string::npos ){
			w += *it;
		}else if( !w.empty() ){
			ret.push_back(w);
			w.clear();
		}
	}
	return ret;
}
// end_code_block
// begin_code_block \WtoString\(
template<class T> inline string toString(T x){ ostringstream out; out<<x; return out.str(); }
// end_code_block
// begin_code_block \WparseNum<
template<class T> inline T parseNum(string s){ T x; istringstream(s)>>x; return x; }
// end_code_block
// begin_code_block \Wsplit<
template<class T> vector<T> split(string s,string d){
	VS t = split(s,d);
	vector<T> ret;
	FOREACH(it,t) ret.push_back(parseNum<T>(*it));
	return ret;
}
// end_code_block
// begin_code_block \WsplitMat<
template<class T> vector<vector<T> > splitMat(vector<string> m,string b){
	vector<vector<T> > ret;
	FOREACH(it,m) ret.push_back(split<T>(*it,b));
	return ret;
}
// end_code_block
// begin_code_block \WDR\[
int DR[]={0,1,0,-1,1,1,-1,-1};// end_code_block
// begin_code_block \WDC\[
int DC[]={1,0,-1,0,1,-1,1,-1};
// end_code_block
// begin_code_block \Wlcm\(
template<class T> T lcm(T a,T b){ return a / std::__gcd(a,b) * b; }
// end_code_block
// begin_code_block \WmakeV\(
template<class T> vector<T> makeV(T a,T b){ vector<T> v(2); v[0] = a; v[1] = b; return v; }
// end_code_block
// begin_code_block \WFraction\W
template<class T>
struct Fraction{
	void norm() { T g = std::__gcd(n,d); n /= g; d /= g; if( d < T(0) ){ n = -n; d = -d; } }
	int n, d;
	Fraction() : n(0), d(1) {}
	Fraction(T n_, T d_) : n(n_), d(d_) { assert( d != T(0) ); norm(); }
	Fraction operator+(const Fraction& f) const { T m = lcm(d, f.d); return Fraction(m/d*n + m/f.d*f.n, m); }
	Fraction operator-() const { return Fraction(-n,d); }
	Fraction operator-(const Fraction& f) const { return *this + (-f); }
	Fraction operator*(const Fraction& f) const { return Fraction(n*f.n, d*f.d); }
	Fraction operator/(const Fraction& f) const { return Fraction(n*f.d, d*f.n); }
	bool operator<(const Fraction& f) const { return n*f.d < f.n*d; }
};
// end_code_block

#define DEBUG(a) cerr<<#a<<'='<<a<<endl
#define DEBUG2(a,b) cerr<<#a<<'='<<a<<','<<#b<<'='<<b<<endl
#define DEBUG3(a,b,c) cerr<<#a<<'='<<a<<','<<#b<<'='<<b<<','<<#c<<'='<<c<<endl
#define DEBUG4(a,b,c,d) cerr<<#a<<'='<<a<<','<<#b<<'='<<b<<','<<#c<<'='<<c<<','<<#d<<'='<<d<<endl
ostream& operator<<(ostream& o,const VS& c){ FOREACH(p,c) o<<*p<<endl; return o; }
template<class T> ostream& operator<<(ostream& o,const vector<T>& c){ FOREACH(p,c) o<<*p<<" "; return o; }
template<class T> ostream& operator<<(ostream& o,const vector<vector<T> >& c){ FOREACH(p,c) o<<*p<<endl; return o; }
template<class U,class V> ostream& operator<<(ostream& o,const pair<U,V>& p){ return o<<'('<<p.first<<','<<p.second<<')'; }
template<class T> ostream& operator<<(ostream& o,const set<T>& c){ FOREACH(p,c) o<<*p<<" "; return o; }
template<class U,class V> ostream& operator<<(ostream& o,const map<U,V>& c){ FOREACH(p,c) o<<*p<<" "; return o; }
template<class T> ostream& operator<<(ostream& o,queue<T> c){ while(!c.empty()) o<<c.front()<<" ",c.pop(); return o; }
template<class T> ostream& operator<<(ostream& os, const Fraction<T>& f){ os << f.n; if(f.d != 1 && f.n != 0) os << '/' << f.d; return os; }
// std::__gcd(a,b), __gnu_cxx::power(a,b), __builtin_popcount(n), __builtin_clz(n), __builtin_ctz(n)


int M,Q;
vector<double> ps;

void run(int tc){
	cin >> M >> Q;
	vector<vector<double> > prob(Q,4);
	REP(q,Q) REP(i,4) cin >> prob[q][i];
	double p = 0;
	int poss = 1 << (Q*2);
	ps.clear();
	REP(d,poss){
		double cur = 1;
		REP(q,Q){
			int a = (d >> (2*q)) % 4;
			cur *= prob[q][a];
		}
		ps.push_back(cur);
	}
	sort(ALL(ps),greater<double>());
	double ret = 0;
	if( M <= poss ){
		REP(i,M) ret += ps[i];
	}else{
		ret = 1;
	}
	cout << "Case #" << tc << ": " << ret << endl;
}
int main(){
	int TC;
	cin >> TC;
	FORE(tc,1,TC) run(tc);
	return 0;
}

