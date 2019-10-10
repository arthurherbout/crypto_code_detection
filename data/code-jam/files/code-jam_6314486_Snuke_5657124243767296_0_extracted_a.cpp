#include <bits/stdc++.h>
#define fi first
#define se second
#define rep(i,n) for(int i = 0; i < (n); ++i)
#define rrep(i,n) for(int i = 1; i <= (n); ++i)
#define drep(i,n) for(int i = (n)-1; i >= 0; --i)
#define each(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();it++)
#define rng(a) a.begin(),a.end()
#define maxs(x,y) x = max(x,y)
#define mins(x,y) x = min(x,y)
#define pb push_back
#define sz(x) (int)(x).size()
#define pcnt __builtin_popcount
#define uni(x) x.erase(unique(rng(x)),x.end())
#define snuke srand((unsigned)clock()+(unsigned)time(NULL));
#define df(x) int x = in()
#define show(x) cout<<#x<<" = "<<x<<endl;
#define PQ(T) priority_queue<T,vector<T>,greater<T> >
#define bn(x) ((1<<x)-1)
#define newline puts("")
#define v(T) vector<T>
#define vv(T) vector<vector<T>>
using namespace std;
typedef long long ll;
typedef unsigned uint;
typedef unsigned long long ull;
typedef pair<int,int> P;
typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<ll> vl;
typedef vector<P> vp;
inline int in() { int x; scanf("%d",&x); return x;}
inline void priv(vi a) { rep(i,sz(a)) printf("%d%c",a[i],i==sz(a)-1?'\n':' ');}
template<typename T>istream& operator>>(istream&i,vector<T>&v)
{rep(j,sz(v))i>>v[j];return i;}
template<typename T>string join(const vector<T>&v)
{stringstream s;rep(i,sz(v))s<<' '<<v[i];return s.str().substr(1);}
template<typename T>ostream& operator<<(ostream&o,const vector<T>&v)
{if(sz(v))o<<join(v);return o;}
template<typename T1,typename T2>istream& operator>>(istream&i,pair<T1,T2>&v)
{return i>>v.fi>>v.se;}
template<typename T1,typename T2>ostream& operator<<(ostream&o,const pair<T1,T2>&v)
{return o<<v.fi<<","<<v.se;}
const int MX = 100005, INF = 1001001001;
const ll LINF = 1e18;
const double eps = 1e-10;
#define dame { puts("-1"); return;}

// coordinate compression
struct X {
  typedef int T;
  vector<T> d;
  void add(T x) { d.pb(x);}
  void init() {
    sort(rng(d));
    d.erase(unique(rng(d)), d.end());
  }
  int size() const { return sz(d);}
  T operator[](int i) const { return d[i];}
  int operator()(T x) const { return upper_bound(rng(d),x)-d.begin()-1;}
};
//

// binary matching
struct match {
  vi used;
  vvi to;
  vi p;
  int n, m;
  match(int n, int m):used(n),to(n),p(m,-1),n(n),m(m){}
  void add(int a, int b) { to[a].pb(b);}
  bool dfs(int v){
    if(used[v]) return false;
    used[v] = 1;
    rep(i,sz(to[v])) {
      int u = to[v][i];
      if (p[u] == -1 || dfs(p[u])){
        p[u] = v;
        return true;
      }
    }
    return false;
  }
  int solve(){
    int res = 0;
    rep(i,n) {
      rep(j,n) used[j] = 0;
      if(dfs(i)) ++res;
    }
    return res;
  }
};
//

struct Solver {
  void solve() {
    int n;
    scanf("%d",&n);
    int m = 6;
    vvi a(n,vi(m));
    cin>>a;
    X xs;
    rep(i,n)rep(j,m) xs.add(a[i][j]);
    xs.init();
    rep(i,n)rep(j,m) a[i][j] = xs(a[i][j]);
    vvi to(sz(xs));
    rep(i,n)rep(j,m) to[a[i][j]].pb(i);
    vvi d;
    int pre = -INF;
    rep(i,sz(xs)) {
      if (xs[i] != pre+1) {
        d.pb({i});
      } else {
        d.back().pb(i);
      }
      pre = xs[i];
    }
    int l = 1, r = n+1;
    while (l+1<r) {
      int c = (l+r)>>1;
      bool ok = false;
      for (vi nd : d) {
        rep(i,sz(nd)-c+1) {
          match g(n,c);
          rep(j,c) {
            for (int v : to[i+j]) g.add(v,j);
          }
          if (g.solve() == c) ok = true;
        }
      }
      if (ok) l = c; else r = c;
    }
    cout<<l<<endl;
  }
};

int main() {
  int ts;
  scanf("%d",&ts);
  rrep(ti,ts) {
    Solver solver;
    printf("Case #%d: ",ti);
    solver.solve();
  }
  return 0;
}




















