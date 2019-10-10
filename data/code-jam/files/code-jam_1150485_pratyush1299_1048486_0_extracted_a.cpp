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
#define EPS 1e-6
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
string arr[105];
int no[105];
int win[105];
int n;
double wp(int I)
{
    return win[I]/(double)(no[I]);
}
double owp(int I)
{
    double ww=0;
    int c=0;
    int i;
    REP(i,n)
    {
        if(i==I) continue;
        if(arr[I][i]=='.') continue;
        ww+=arr[I][i]=='1'?(win[i])/(double)(no[i]-1):(win[i]-1)/(double)(no[i]-1);
        c++;
    }
    return ww/(double)c;
}
double oowp(int I)
{
    double ww=0;
    int c=0;
    int i;
    REP(i,n)
    {
        if(I==i) continue;
        if(arr[I][i]=='.') continue;
        ww+=owp(i);
        c++;
    }
    return ww/(double)c;
}
int main()
{
    int i,j,k,p,q;
    p=0;
    cin>>q;
    while(q--)
    {
        cin>>n;
        MEM(no,0);
        MEM(win,0);
        REP(i,n)
        {
            cin>>arr[i];
            REP(j,n) {
                if(arr[i][j]=='1') win[i]++;
                if(arr[i][j]!='.') no[i]++;
            }
        }
        cout<<"Case #"<<++p<<":\n";
        REP(i,n) {
//            cout<<"wp: "<<wp(i)<<endl;
  //          cout<<"owp: "<<owp(i)<<endl;
    //        cout<<"oowp: "<<oowp(i)<<endl;
            double per=(0.25*wp(i))+(0.5*owp(i))+(0.25*oowp(i));
            printf("%0.6lf\n",per);
        }
    }
//	system("pause");
	return 0;
}



























