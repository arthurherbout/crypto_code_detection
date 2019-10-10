#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <ctime>
#include <queue>
#include <stack>
#include <algorithm>
#include <list>
#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <deque>
#include <complex>
#include <string>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <valarray>
#include <unordered_map>
#include <iterator>
#include <functional>
#include <cassert>
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

const ll INF = 1ll<<45;
const double EPS = 1e-8;
const int MOD = 1000000007;


int T, n, k;

int main(int argc, char *argv[]){
	ios::sync_with_stdio(false);
	cin >> T;
	REPS(t, T){
cerr << t << "/" << T << endl;
		cin >> n >> k;
		vector<ll> x(n);
		cin >> x[0];
		ll p = x[0];
		REP(i, n-k){
			ll y;
			cin >> y;
			x[i+k] = x[i] + y - p;
			p = y;
		}
//		cout << x << endl;
		vector<pll> range(k, pll(INF, -INF));
		REP(i, n){
			chmin(range[i%k].first , x[i]);
			chmax(range[i%k].second, x[i]);
		}
		ll suml = 0;
//		cout << range << endl;
		sort(ALL(range), [](const pll &x, const pll &y){
			return x.second - x.first < y.second - y.first;
		});
		REP(i, k) suml += range[i].first;
		ll ans = range[k-1].second - range[k-1].first;
		suml = (suml % k + k) % k;
		REP(i, k){
			ll t = range[i].second - range[i].first;
			suml -= ans - t;
//			range[i] = pll(range[i].first + d, range[i].second + d);
		}
		if(suml > 0) ans ++;
//		cout << range << endl;
		printf("Case #%d: %lld\n", t, ans);
//		cout << "Case #" << t << ": " << ans << endl;
	}
	return 0;
}
