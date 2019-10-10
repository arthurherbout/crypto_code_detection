/*
ID: vermapr1
PROG: <prog_name>
LANG: C++
*/

// #define TRUE FALSE haha now debug this
using namespace std;
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <fstream>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
//BEGIN_TEMPLATE_BY_PRATYUSH_VERMA

//CONSTANT
#define INF (1<<31)-1
#define PI 3.1428571428571428571428571428571

//FUNC
#define MAX(i,j) (i)>(j)?(i):(j)
#define MIN(i,j) (i)<(j)?(i):(j)
#define REP(i,a) for((i)=0;(i)<(a);(i)++)
#define REP_(i,a) for((i)=0;(i)<=(a);(i)++)
#define FOR(i,a,b) for((i)=(a);(i)<(b);(i)++)
#define FOR_(i,a,b) for((i)=(a);(i)<=(b);(i)++)
#define VAR(V,init) __typeof(init) V=(init)
#define FOREACH(I,C) for(VAR(I,(C).begin());I!=(C).end();I++)
#define ALL(x) (x).begin(),(x).end()
#define SIZE(x) ((int)(x.size()))
#define LENGTH(x) ((int)(x.length()))
#define SZ(x) sizeof(x)
#define MEM(m,i) memset((m),(i),SZ(m))
#define PB(x,y) (x).push_back(y)
#define MP(x,y) make_pair(x,y)
#define V(x) vector < x >

//BITWISE
#define LASTSETBIT(x) x & ~(x-1)
#define TRAILZERO(x) __builtin_ctz(x)
#define LEADZERO(x) __builtin_clz(x)
#define COUNTSETBIT(x) __builtin_popcount(x)
#define GCD(x,y) __gcd(x,y)

typedef pair<int,int>   PII;
typedef pair<char,int>  PCI;
typedef pair<int,PII>   TRI;
typedef V( int )        VI;
typedef V( PII )        VII;
typedef V( TRI )        VTRI;
typedef V( string )     VS;
typedef long long       LL;
typedef long double     LD;

template<class T> void print(const vector<T> &v){ostringstream os; for(int i=0; i<v.size(); ++i){if(i)os<<' ';os<<v[i];} cout<<os.str()<<endl;}
template<class T, class U> T cast (U x) { ostringstream os; os<<x; T res; istringstream is(os.str()); is>>res; return res; }
template<class T> vector<T> split(string s, string x=" ") {vector<T> res; for(int i=0;i<s.size();i++){string a; while(i<(int)s.size()&&x.find(s[i])==string::npos)a+=s[i++]; if(!a.empty())res.push_back(cast<T>(a));} return res;}

inline void input( int &n ) { n=0; int ch=getchar(); while( ch < '0' || ch > '9' ) ch=getchar(); while( ch >= '0' && ch <= '9' ) n = (n<<3)+(n<<1) + ch-'0', ch=getchar(); }

//END_TEMPLATE_BY_PRATYUSH_VERMA
int main()
{
    int n,l,h,t;
    cin>>t;
    int i,j,k,L,H,p,q,cs=0;
    while(t--)
    {
        cout<<"Case #"<<++cs<<": ";
        cin>>n>>L>>H;
        if(L==1 || H==1) { cout<<"1\n"; continue; }
        vector<int> v;
        vector<int> g;
        vector<int> l;
        REP(i,n) {
            cin>>p;
            PB(v,p);
        }
        int gg=v[0],ll=v[0];
        FOR(i,1,n) {
            gg=GCD(gg,v[i]);
        }
        if(gg!=1 && gg<=H && gg>=L) {
            cout<<gg<<endl;
            continue;
        }
        FOR(i,1,n) {
            ll=(ll*v[i])/gg;
        }
        if(ll<=H && ll>=L) {
            cout<<ll<<endl;
            continue;
        }
        sort(ALL(v));
//        PB(l,v[0]);
        gg=v[0];
        ll=v[0];
        PB(l,ll);
        FOR(i,1,n) {
            gg=GCD(gg,v[i]);
            ll=(ll*v[i])/gg;
            PB(l,ll);
        }
  //      PB(g,v[n-1]);
        reverse(ALL(v));
        ll=v[0];gg=v[0];
        PB(g,gg);
        FOR(i,1,n) {
            gg=GCD(gg,v[i]);
            PB(g,gg);
        }
  //      PB(g,1);
        reverse(ALL(g));
      //  REP(i,g.size()) cout<<g[i]<<" ";
    //    cout<<endl;
  //      REP(i,l.size()) cout<<l[i]<<" ";
//        cout<<endl;
        FOR(i,0,l.size()) {
            if(l[i]%g[i]==0 && g[i]!=1 && l[i]<=H && l[i]>=L) break;
        }


        if(i!=l.size()) cout<<l[i]<<endl;
        else cout<<"NO\n";
    }
//	system("pause");
	return 0;
}





























