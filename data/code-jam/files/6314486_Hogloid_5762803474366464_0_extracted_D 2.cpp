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
typedef long double ld;


namespace std{
  template<class S,class T>
  ostream &operator <<(ostream& out,const pair<S,T>& a){
    out<<'('<<a.fr<<','<<a.sc<<')';
    return out;
  }
}

//const int INF=5e8;

int Tnum;
struct pt{
  ld x,y,z;
  ld dot(const pt& X){
    return x*X.x+y*X.y+z*X.z;
  }
  ld norm(){
    return sqrt(max(0.0L,x*x+y*y+z*z));
  }
  pt mult(ld a){
    pt res;
    res.x=x*a;res.y=y*a;res.z=z*a;
    return res;
  }
  pt add(pt a){
    pt res;
    res.x=x+a.x;res.y=y+a.y;res.z=z+a.z;
    return res;
  }
};
const ld EPS=1e-15;
class Main{
  public:
  int n;
  pt pts[5005];
  void input(){
    cin>>n;
    REP(i,n){
      cin>>pts[i].x>>pts[i].y>>pts[i].z;
      pts[i]=pts[i].mult(sqrt(pts[i].norm()));
    }
  }
  bool suc;
  void operator()(){
    pt vec;
    vec.x=vec.y=vec.z=1;

    suc=true;
    ld mergin=1;
    REP(hoge,1000000) {
      bool fail = false;
      REP(i, n) {
        ld val = vec.dot(pts[i]);
        if (val < EPS) {
          ld need = sqrt(-val+mergin);
          vec = vec.add(pts[i].mult(need));
          fail = true;
          break;
        }
      }
      vec=vec.mult(1.0/sqrt(vec.norm()));
      if (!fail){
        suc = false;
        break;
      }
      mergin*=0.999;
      chmax(mergin,EPS);
    }
    int piyo=0;
  }
  void output(){
    puts(!suc?"NO":"YES");
  }
};

const int MAX_T=205;
Main solver[MAX_T];
int main(){
#ifdef LOCAL_REDIR
  freopen("/home/codejam/CLionProjects/codejam/in.txt","r",stdin);
#endif
#ifdef OUT_REDIR
  freopen("/home/codejam/CLionProjects/codejam/out.txt","w",stdout);
#endif
  cin>>Tnum;
  for(int i=0;i<Tnum;++i){
    solver[i].input();
  }
  vector<thread> threads;
  for(int i=0;i<Tnum;++i){
    threads.push_back(thread(ref(solver[i])));
  }
  for(auto& t:threads){
    t.join();
  }
  for(int i=0;i<Tnum;++i){
    printf("Case #%d: ",i+1);
    solver[i].output();
  }
  return 0;
}
