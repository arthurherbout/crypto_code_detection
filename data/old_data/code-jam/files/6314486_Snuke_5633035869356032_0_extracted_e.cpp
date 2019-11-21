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
typedef vector<vp> vvp;
typedef vector<vvp> vvvp;
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

int pn;
vvp ps;

struct Solver {
  int n;
  vvp pro(vvp a) {
    while (1) {
      vvp s(n);
      rep(i,n) {
        if (sz(a[i])) {
          P p = a[i].back();
          s[p.se].pb(P(p.fi,i));
        }
      }
      bool up = false;
      rep(i,n) {
        if (sz(s[i]) > 1) {
          up = true;
          sort(rng(s[i]));
          int j = s[i][0].se;
          a[j].pop_back();
        }
      }
      if (!up) return a;
    }
  }
  bool end(vvp& a) {
    rep(i,sz(a)) if (sz(a[i]) > 1) return false;
    return true;
  }
  P emp(vvp& a) {
    int cnt = 0, ei = -1;
    rep(i,n) if (sz(a[i]) == 0) ei = i, cnt++;
    return P(ei,cnt);
  }
  bool dfs(vvp a) {
    a = pro(a);
    if (end(a)) return true;
    P e = emp(a);
    if (e.fi == -1) return false;

    vvvp bs;
    rep(i,n) {
      if (sz(a[i]) > 1) {
        vvp b = a;
        b[e.fi].pb(b[i].back());
        b[i].pop_back();
        b = pro(b);
        // if (emp(b).se == e.se) return dfs(b);
        bs.pb(b);
      }
    }
    for (vvp& b : bs) if (dfs(b)) return true;
    return false;
  }
  void solve() {
    int c;
    scanf("%d%d",&n,&c);
    vvp a;
    a = vvp(n);
    X xs;
    rep(i,n) {
      int j;
      scanf("%d",&j);
      a[i] = ps[j];
      for (auto p : a[i]) xs.add(p.se);
    }
    xs.init();
    if (sz(xs) < n) {
      cout<<"POSSIBLE"<<endl;
      return;
    }
    if (sz(xs) > n) {
      cout<<"IMPOSSIBLE"<<endl;
      return;
    }
    rep(i,n)rep(j,c) a[i][j].se = xs(a[i][j].se);
    // vi s(4);
    puts(dfs(a)?"POSSIBLE":"IMPOSSIBLE");
  }
};

int main() {
  scanf("%d",&pn);
  rep(i,pn) {
    int c;
    scanf("%d",&c);
    ps.pb(vp(c));
    rep(j,c) scanf("%d%d",&ps[i][j].fi,&ps[i][j].se);
    reverse(rng(ps[i]));
  }
  int ts;
  scanf("%d",&ts);
  rrep(ti,ts) {
    Solver solver;
    printf("Case #%d: ",ti);
    solver.solve();
  }
  return 0;
}




















