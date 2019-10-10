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
#define maxn 1100
#define maxp 1100000

//ifstream fin("data.in");
//#define cin fin

int n,I;
map<string,int> M;
struct tree {
  vector<int> child;
  int num;
  string name;
};
int get(string s) {
  if(M.find(s)==M.end()) {
    M[s]=I;
    I++;
  }
  return M[s];
}
tree data[maxn];
int F[maxn],maxx;

int f(int i) {
  vector<int> V;
  REP(j,data[i].child.size()) {
    V.push_back(f(data[i].child[j]));
  }
  sort(V.rbegin(),V.rend());
  maxx >?= V.size()+1;
  while(1) {
    int ind = maxx;
    bool flag = true;
    REP(j,V.size()) {
      if(V[j]>=ind) {
	flag=false;
	break;
      }
      ind--;
    }
    if(flag) break;
    else maxx++;
  }
  return V.size();
}

int main(){
  int T;
  cin>>T;
  for(int C=1; C<=T; C++) {
    M.clear();
    I=0;
    cin>>n;
    REP(i,n) {
      string s;
      int m;
      cin>>s>>m;
      int cur = get(s);
      data[cur].name=s;
      data[cur].child.clear();
      data[cur].num=0;
      REP(j,m) {
	cin>>s;
	if(isupper(s[0])) data[cur].child.push_back(get(s));
	else data[cur].num++;
      }
    }
    maxx=1;
    f(0);
    cout << "Case #"<<C<<": "<<maxx<<endl;
  }
}
