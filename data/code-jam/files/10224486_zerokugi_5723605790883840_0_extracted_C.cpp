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

const int INF = 1<<28;
const double EPS = 1e-8;
const int MOD = 1000000007;

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
		vi match(2*(n + m));
		REP(i, n + m){
			int u, v;
			cin >> u >> v; u--; v--;
			match[u] = v;
			match[v] = u;
		}
		
		auto ver = [&](int y, int x){
			return x + (m+1) * y;
		};
		auto hor = [&](int y, int x){
			return n * (m+1) + x + m * y;
		};
		int N = n * (m+1) + (n+1) * m;
		
		vi place;
		REP(i, m) place.eb(hor(0, i));
		REP(i, n) place.eb(ver(i, m));
		RREP(i, m) place.eb(hor(n, i));
		RREP(i, n) place.eb(ver(i, 0));
		int ans = [&](){
			REP(s, 1 << (n*m)){
				UnionFind uf(N);
				REP(i, n)REP(j, m){
					int b = 1 & (s >> (i*m + j));
					if(b){
						uf.unite(hor(i, j), ver(i, j+1));
						uf.unite(ver(i, j), hor(i+1, j));
					}else{
						uf.unite(hor(i, j), ver(i, j));
						uf.unite(ver(i, j+1), hor(i+1, j));
					}
				}
				if([&](){
					REP(i, match.size())if(!uf.find(place[i], place[match[i]])) return 0;
					return 1;
				}()) return s;
			}
			return -1;
		}();
		cout << "Case #" << t << ":" << endl;
		if(ans == -1) cout << "IMPOSSIBLE" << endl;
		else{
			REP(i, n){
				REP(j, m) cout << "/\\"[1 & (ans >> (i*m+j))];
				cout << endl;
			}
		}
	}
	return 0;
}
