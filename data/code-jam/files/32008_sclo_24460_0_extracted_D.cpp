#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <complex>
#include <cstdio>
#include <cassert>
#include <cmath>

#if defined (__GNUC__) && (__GNUC__ <= 2)
#include <hash_map>
#include <hash_set>
#else
#include <ext/hash_map>
#include <ext/hash_set>
using namespace __gnu_cxx;
#endif
using namespace std;

#define FOR(i,a,b) for(int i=(a),_b=(b);i<=_b;i++)
#define REP(i,n) FOR(i,0,(n)-1)
#define FORD(i,a,b) for(int i=(a),_b=(b);i>=_b;i--)
#define sz size()
template<class T> inline int size(const T& c) { return c.sz; }
#define FORA(i,c) REP(i,size(c))

#define itype(c) __typeof((c).begin())
#define FORE(e,c) for(itype(c) e=(c).begin();e!=(c).end();e++)
#define pb push_back
#define X first
#define Y second
#define mp make_pair
#define all(x) (x).begin(),(x).end()
#define SORT(x) sort(all(x))
#define REVERSE(x) reverse(all(x))
#define CLEAR(x,c) memset(x,c,sizeof(x)) 

typedef long long LL;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
LL s2i(string s) { istringstream i(s); LL x; i>>x; return x; }
template<class T> string i2s(T x) { ostringstream o; o << x; return o.str(); }

#define pi acos(-1.)
#define eps 1e-7
#define inf 1000000000
#define maxn 4
#define maxp 1100000

//ifstream fin("data.in");
//#define cin fin

int B[maxn][maxn];
int m,n;
int di[] = {-1,-1,-1,0,0,1,1,1};
int dj[] = {-1,0,1,-1,1,-1,0,1};
bool inside(int i,int j) {
  return i>=0 && i<m && j>=0 && j<n;
}
#define c(i,j) ((i)*n+(j))

int F[4][4][1<<16];

int f(int i,int j,int mask) {
  int& ret = F[i][j][mask];
  if(ret==-1) {
    ret = 0;
    REP(d,8) {
      int ii=i+di[d], jj=j+dj[d];
      if(inside(ii,jj)&&(mask&(1<<c(ii,jj)))==0) {
	int nmask = mask|(1<<c(ii,jj));
	if(!f(ii,jj,nmask)) ret=1;
	/*
	if(c(i,j)==3 && mask==15&&ret==1) {
	  cout << "ad: "<<ii<<' '<<jj<<' ';
	  cout << inside(ii,jj)<<' '<<c(ii,jj)<<' ';
	  cout << (mask&(1<<c(ii,jj)))<<endl;
	}
	*/
      }
    }
    //cout << c(i,j)<<' '<<mask<<' '<<ret << endl;
  }
  return ret;
}

int main(){
  int T,si,sj;
  cin>>T;
  for(int C=1; C<=T; C++) {
    cin>>m>>n;
    REP(i,m) REP(j,n) {
      char c;
      cin>>c;
      if(c=='K') B[i][j]=1,si=i,sj=j;
      else if(c=='#') B[i][j]=1;
      else B[i][j]=0;
    }
    memset(F,-1,sizeof(F));
    /*
    REP(i,m) {
      REP(j,n) cout << B[i][j];
      cout << endl;
      }*/
    int mask = 0;
    REP(i,m) REP(j,n) if(B[i][j]) mask|=1<<c(i,j);
    //cout << mask << endl;
    int sol = f(si,sj,mask);
    char ans = sol?'A':'B';
    cout << "Case #"<<C<<": "<<ans<<endl;
  }
}
