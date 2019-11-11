#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned long long ull;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;

#define REP(i,x) for(int i=0;i<(int)(x);i++)
#define REPS(i,x) for(int i=1;i<=(int)(x);i++)
#define RREP(i,x) for(int i=((int)(x)-1);i>=0;i--)
#define RREPS(i,x) for(int i=((int)(x));i>0;i--)
#define FOR(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();i++)
#define RFOR(i,c) for(__typeof((c).rbegin())i=(c).rbegin();i!=(c).rend();i++)
#define ALL(container) (container).begin(), (container).end()
#define RALL(container) (container).rbegin(), (container).rend()
#define SZ(container) ((int)container.size())
#define mp(a,b) make_pair(a, b)
#define pb push_back
#define eb emplace_back
#define UNIQUE(v) v.erase( unique(v.begin(), v.end()), v.end() );

template<class T> bool chmax(T &a, const T &b) { if (a<b) { a=b; return 1; } return 0; }
template<class T> bool chmin(T &a, const T &b) { if (a>b) { a=b; return 1; } return 0; }
template<class T> ostream& operator<<(ostream &os, const vector<T> &t) {
os<<"["; FOR(it,t) {if(it!=t.begin()) os<<","; os<<*it;} os<<"]"; return os;
}
template<class T> ostream& operator<<(ostream &os, const set<T> &t) {
os<<"{"; FOR(it,t) {if(it!=t.begin()) os<<","; os<<*it;} os<<"}"; return os;
}
template<class S, class T> ostream& operator<<(ostream &os, const pair<S,T> &t) { return os<<"("<<t.first<<","<<t.second<<")";}
template<class S, class T> pair<S,T> operator+(const pair<S,T> &s, const pair<S,T> &t){ return pair<S,T>(s.first+t.first, s.second+t.second);}
template<class S, class T> pair<S,T> operator-(const pair<S,T> &s, const pair<S,T> &t){ return pair<S,T>(s.first-t.first, s.second-t.second);}


#define EPS 1.0e-10
inline int signum(double x) { return (abs(x) < EPS ? 0 : x > 0 ? 1 : -1); }

struct P{
	double x, y, z;
	P():x(0),y(0),z(0){}
	P(double a, double b, double c):x(a),y(b),z(c){}
	
	P operator+(const P &opp){
		return P(x+opp.x, y+opp.y, z+opp.z);
	}
	P operator-(const P &opp){
		return P(x-opp.x, y-opp.y, z-opp.z);
	}
	P operator/(const P &opp){
		return P(abs(opp.x)<EPS?0:x/opp.x, abs(opp.y)<EPS?0:y/opp.y, abs(opp.z)<EPS?0:z/opp.z);
	}
	P operator/(const double &opp){
		return P(x/opp, y/opp, z/opp);
	}
	P operator*(const double opp){
		return P(x*opp, y*opp, z*opp);
	}
	friend ostream& operator<<(ostream &os, const P &t) { return os<<"("<<t.x<<","<<t.y<<","<<t.z<<")";}
};

struct L{
	P pos, dir;
	L(){}
	L(P p, P d):pos(p),dir(d){}
	L(P d):pos(),dir(d){}
};

struct C{
	P p;
	double r;
	C(){}
	C(P q, double s):p(q),r(s){}
};

inline double inp(P v1, P v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline P outp(P p1, P p2){
	return P(p1.y*p2.z - p1.z*p2.y, p1.z*p2.x - p1.x*p2.z, p1.x*p2.y - p1.y*p2.x);
}

inline double norm(P v){
	return v.x*v.x + v.y*v.y + v.z*v.z;
}
inline double abs(P v){
	return sqrt(norm(v));
}

inline P perf(P v1, L l){
	return l.pos + l.dir*(inp(v1-l.pos, l.dir)/norm(l.dir));
}

bool lp_intersect(L l, P t){	// “_‚ª’¼üã‚É‚ ‚é‚©‚Ç‚¤‚©
	return !signum(norm(outp(t-l.pos, l.dir)));
}

bool sp_intersect(L l, P t){	// “_‚ªü•ªã‚É‚ ‚é‚©‚Ç‚¤‚©
	P a(t-l.pos), b(l.dir);
	if(signum(norm(a)) == 0 || signum(norm(a-b)) == 0) return true;	// ’[“_
	if(signum(norm(outp(a, b)))) return false;	// ’¼üã‚É‚È‚¢
	if(signum(a.x*b.x) == -1 || signum(a.y*b.y) == -1 || signum(a.z*b.z) == -1) return false;	// ‹t•ûŒü
	if(norm(a) < norm(b)) return true;
	return false;
}

double lp_distance(L l, P t){
	P v1 = t-l.pos;
	P v2 = perf(v1, L(l.dir));
	return abs(v1 - v2);
}

double sp_distance(L l, P t){
	P v1 = t-l.pos;
	P v2 = perf(v1, L(l.dir));
	if(sp_intersect(L(l.dir), v2)) return abs(v1 - v2);
	else return sqrt(min(norm(t-l.pos), norm(t-(l.pos + l.dir))));
}

double sp_distance(P t, P p1, P p2){
	return sp_distance(L(p1, p2-p1), t);
}

vector<P> cl_crosspoint(C c, L l){
	P p = perf(c.p, l);
	double d2 = norm(c.p-p);
	if(d2 > c.r*c.r + EPS) return vector<P>();
	if(d2 > c.r*c.r - EPS) return vector<P>(1, p);	//‹«ŠE
	double s = sqrt(c.r*c.r - d2);
	vector<P> ret;
	ret.push_back(p + l.dir/abs(l.dir)*s);
	ret.push_back(p - l.dir/abs(l.dir)*s);
	return ret;
}

vector<P> cs_crosspoint(C c, L l){
	vector<P> ret, res = cl_crosspoint(c, l);
	REP(i, res.size()) if(sp_intersect(l, res[i])) ret.push_back(res[i]);
	return ret;
}

bool cl_intersect(C c, L l){
	return lp_distance(l, c.p) < c.r + EPS;	// ‹«ŠE‚ðŠÜ‚Þ
}

bool cs_intersect(C c, L l){
	return !cs_crosspoint(c, l).empty();
}

P reflect(P p, L l){
	P q = perf(p, l);
	return q + (q - p);
}

int T, n, m;

struct UnionFind {
	vector<int> d;
	UnionFind(int n=0) : d(n, -1){}
	bool unite(int x, int y) {
		if ((x = root(x)) != (y = root(y))){
			if (d[y] < d[x]) swap(x, y);
			d[x] += d[y]; d[y] = x;
		}
		return x != y;
	}
	bool find(int x, int y){return root(x) == root(y);}
	int root(int x){return d[x] < 0 ? x : d[x] = root(d[x]);}
	int size(int x=-1) {return x<0 ? d.size() : -d[root(x)];}
};


int main(int argc, char *argv[]){
	ios::sync_with_stdio(false);
	cin >> T;
	REPS(t, T){
		cin >> n >> m;
		vector<P> p, v;
		REP(i, n){
			double x, y, z;
			cin >> x >> y >> z;
			p.eb(x, y, z);
			cin >> x >> y >> z;
			v.eb(x, y, z);
		}
		
		double l=0, r=1e9;
		REP(_, 100){
			double mid = (l+r)/2;
			if([&](){
				UnionFind uf(n);
				REP(i, n)REP(j, i){
					if(abs(p[i] - p[j]) <= mid) uf.unite(i, j);
				}
				return uf.find(0, 1);
			}()) r = mid;
			else l = mid;
		}
		printf("Case #%d: %.7f\n", t, r);
	}
	return 0;
}
