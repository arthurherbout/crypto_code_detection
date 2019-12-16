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
typedef vector<vvi> vvvi;
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
const int MX = 10001, INF = 1001001001;
const ll LINF = 1e18;
const double eps = 1e-10;
#define dame { puts("-1"); return;}

// Union find
struct uf {
  vi d;
  uf(){}
  uf(int mx):d(mx,-1){}
  int root(int x) {
    if(d[x] < 0) return x;
    return d[x] = root(d[x]);
  }
  bool unite(int x, int y) {
    x = root(x); y = root(y);
    if(x == y) return false;
    if(d[x] > d[y]) swap(x,y);
    d[x] += d[y]; d[y] = x;
    return true;
  }
  int size(int x) { return -d[root(x)];}
};
//

void pri(vvi& g) {
  rep(i,sz(g)) {
    rep(j,sz(g[i])) printf("%d",g[i][j]);
    newline;
  }
}


vvvi gs;

int n;
vvi used;
vvi d(4);
map<vi,vi> mp;
void dfs(int a, int b, int dep=0, int x=-1, int pa=-1, int pb=-1) {
  if (dep > n) return;
  if (a >= MX) return;
  if (mp.count({a,b,dep})) return;
  mp[{a,b,dep}] = {pa,pb,x};
  // if (used[dep][a]) return;
  used[dep][a] = b;
  dfs(a+a+b,a+b,dep+1,1,a,b);
  dfs(a,a+b,dep+1,2,a,b);
  dfs(a,a,dep+1,3,a,b);
  dfs(a*5+b*3,a*2+b*3,dep+2,6,a,b);
  dfs(a*8+b*8,a*3+b*5,dep+2,7,a,b);
  // dfs(a*3+b,a*2+b,dep+2,7);
  // dfs(0,a,dep+1,4);
  // dfs(a*4+b*4,a*4+b*4,dep+2,4);
  // dfs(a*8+b*8,a*4+b*4,dep+2,5);
}

void add(vvi& g, int a, int b) { g[a][b] = g[b][a] = 1;}

vvi gfs(int dep, int a, int b) {
  if (dep == 0) {
    return {{0,1},{1,0}};
  }
  vi pre = mp[{a,b,dep}];
  int x = pre[2];
  int ds = 1;
  if (x >= 6) ds = 2;
  vvi g = gfs(dep-ds,pre[0],pre[1]);
  rep(i,sz(g)) g[i].pb(0);
  g.pb(vi(dep+2));
  // cout<<dep<<" "<<a<<" "<<b<<endl;pri(g);
  int v = sz(g)-1;
  int s = sz(g)-2;
  int t = sz(g)-3;
  if (x == 1) {
    add(g,s,v);
    add(g,t,v);
  } else if (x == 2) {
    add(g,t,v);
  } else if (x == 3) {
    add(g,s,v);
  } else {
    v++; s++; t++;
    rep(i,dep) g[i].insert(g[i].begin(),0);
    g.insert(g.begin(),vi(dep+2));
    int w = 0;
    add(g,s,v);
    add(g,t,v);
    add(g,t,w);
    add(g,w,v);
    if (x == 7) add(g,s,w);
  }
  return g;
}

struct Solver {
  void solve() {
    int m;
    scanf("%d",&m);
    if (sz(gs[m])) {
      cout<<sz(gs[m])<<endl;
      pri(gs[m]);
      return;
    }

    rep(dep,n+1) if (used[dep][m] != -1) {
      int b = used[dep][m];
      vvi g = gfs(dep,m,b);
      cout<<sz(g)<<endl;
      pri(g);
      return;
    }
    cout<<0<<endl;
    cerr<<m<<endl;
  }
};

void gen(int n) {
  vp e;
  rep(i,n)rep(j,i) e.pb(P(i,j));
  random_shuffle(rng(e));
  int m = sz(e);
  mins(m,20);
  vi dp(1<<m);
  rep(i,1<<m) if (pcnt(i) == n-1) {
    uf t(n);
    bool ok = true;
    rep(j,m) if (i>>j&1) {
      if (!t.unite(e[j].fi,e[j].se)) {
        ok = false;
        break;
      }
    }
    dp[i] = ok;
  }
  rep(i,m)rep(j,1<<m) if (!(j>>i&1)) dp[j|1<<i] += dp[j];
  rep(i,1<<m) if (dp[i] < MX && sz(gs[dp[i]]) == 0) {
    vvi g(n,vi(n));
    rep(j,m) if (i>>j&1) add(g,e[j].fi,e[j].se);
    gs[dp[i]] = g;
  }
}

int main() {
  n = 20;
  // n = 10;
  used = vvi(n+1,vi(MX,-1));
  dfs(1,1); //dfs(0,1);
  // rep(i,MX)rep(j,n) if (used[j][i]) {
  //   for (int k = 2; k <= j+2; ++k) {
  //     if (i*k >= MX) break;
  //     used[j+1][i*k] = MX+k;
  //   }
  // }

  gs = vvvi(MX);

  while (1) {
    gen(10);

    //*
    int cnt = 0;
    rep(i,MX) if (i >= 3) {
      bool ok = false;
      rep(j,n+1) if (used[j][i] != -1) ok = true;
      if (sz(gs[i])) ok = true;
      if (!ok) cerr<<i<<endl;
      if (!ok) cnt++;
    }
    cerr<<"fail:"<<cnt<<endl;

    if (cnt == 0) break;
    // return 0;
  }
  //*/

  int ts;
  scanf("%d",&ts);
  rrep(ti,ts) {
    Solver solver;
    printf("Case #%d: ",ti);
    solver.solve();
  }
  return 0;
}




















