#define DEB
#include <bits/stdc++.h>
#include <thread>
#define REP(i,m) for(int i=0;i<(m);++i)
#define REPN(i,m,in) for(int i=(in);i<(m);++i)
#define ALL(t) (t).begin(),(t).end()
#define CLR(a) memset((a),0,sizeof(a))
#define pb push_back
#define mp make_pair
#define fr first
#define sc second

using namespace std;


#ifdef DEB
#define dump(x)  cerr << #x << " = " << (x) << endl
#define prl cerr<<"called:"<< __LINE__<<endl
#define dumpR(x) cerr<<"\x1b[31m"<<#x<<" = " <<(x)<<"\x1b[39m"<<endl
#define dumpY(x) cerr<<"\x1b[33m"<<#x<<" = " <<(x)<<"\x1b[39m"<<endl
#define dumpG(x) cerr<<"\x1b[32m"<<#x<<" = " <<(x)<<"\x1b[39m"<<endl
template<class T> void debug(T a,T b){ for(;a!=b;++a) cerr<<*a<<' ';cerr<<endl;}
#else
#define dump(x) ;
#define dumpR(x) ;
#define dumpY(x) ;
#define dumpG(x) ;
#define prl ;
template<class T> void debug(T a,T b){ ;}
#endif

template<class T> void chmin(T& a,const T& b) { if(a>b) a=b; }
template<class T> void chmax(T& a,const T& b) { if(a<b) a=b; }

typedef long long int lint;
typedef pair<int,int> pi;

namespace std{
  template<class S,class T>
  ostream &operator <<(ostream& out,const pair<S,T>& a){
    out<<'('<<a.fr<<','<<a.sc<<')';
    return out;
  }
}

//const int INF=5e8;

int m;
int len[60005];
pi stk[60005][15];

typedef vector<pi> vp;
map<vp,bool> done;//(stack id, height) or (-value-1,suit)

bool rec(vp X){
  sort(ALL(X));
  if(done.count(X)) return done[X];

  bool& res=done[X];
  int n=X.size();
  pi top[4];memset(top,-1,sizeof(top));

  REP(i,n){
    if(X[i].fr>=0){
      if(X[i].sc>0) top[i]=stk[X[i].fr][X[i].sc-1];
    }else{
      top[i]={-X[i].fr-1,X[i].sc};
    }
  }
  int exi=0;
  REP(i,n){
    if(X[i].fr<0 || X[i].sc<=1){
      ++exi;
    }
  }
  if(exi==n) return res=true;
  REP(i,n) if(top[i].fr>=0){
    bool same=false,smlest=true;
    REP(j,n) if(i!=j && top[i].sc==top[j].sc) same=true;
    REP(j,n) if(top[i].fr>top[j].fr && top[i].sc==top[j].sc) smlest=false;
    if(same && smlest){
      vp Y=X;
      if(X[i].fr<0) Y[i]={0,0};
      else Y[i].sc--;
      if(rec(Y)) return res=true;
    }
  }
  REP(i,n) if(top[i].fr>=0){
      REP(j,n) if(top[j].fr==-1){
          vp Y=X;
          if(X[i].fr>=0) --Y[i].sc;
          else Y[i]={0,0};
          Y[j]={-top[i].fr-1,top[i].sc};
          if(rec(Y)) return res=true;
        }
  }
  return res=false;
}
int main(){
#ifdef LOCAL_REDIR
  freopen("/home/codejam/CLionProjects/codejam/in.txt","r",stdin);
#endif
#ifdef OUT_REDIR
  freopen("/home/codejam/CLionProjects/codejam/out.txt","w",stdout);
#endif
  int Tnum;
  cin>>m;
  REP(i,m){
    cin>>len[i];
    REP(j,len[i]){
      int a,b;cin>>a>>b;
      stk[i][j]={a,b};
    }
    reverse(stk[i],stk[i]+len[i]);
  }
  cin>>Tnum;
  for(int i=1;i<=Tnum;++i){
    done.clear();
    int n,C;
    cin>>n>>C;
    vp init(n);
    REP(i,n){
      int a;cin>>a;
      init[i]={a,C};
    }
    printf("Case #%d: ",i);
    if(rec(init)) puts("POSSIBLE");
    else puts("IMPOSSIBLE");
  }
  return 0;
}
