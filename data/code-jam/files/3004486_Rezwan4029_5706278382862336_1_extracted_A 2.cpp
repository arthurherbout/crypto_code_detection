/*
    Rezwan_4029 , AUST
*/

#include <bits/stdc++.h>

#define pb push_back
#define all(x) x.begin(),x.end()
#define ms(a,v) memset(a,v,sizeof a)
#define II ({int a; scanf("%d", &a); a;})
#define LL ({Long a; scanf("%lld", &a); a;})
#define DD ({double a; scanf("%lf", &a); a;})
#define ff first
#define ss second
#define mp make_pair
#define gc getchar
#define EPS 1e-10
#define pi 3.1415926535897932384626433832795
using namespace std;

#define FI freopen ("in.txt", "r", stdin)
#define FO freopen ("out.txt", "w", stdout)

typedef long long Long;
typedef unsigned long long ull;
typedef vector<int> vi ;
typedef set<int> si;
typedef vector<Long>vl;
typedef pair<int,int>pii;
typedef pair<string,int>psi;
typedef pair<Long,Long>pll;
typedef pair<double,double>pdd;
typedef vector<pii> vpii;

int cs = 1 ;

#define CS printf("Case #%d:",cs++)

#define forab(i, a, b)	for (__typeof (b) i = (a) ; i <= b ; ++i)
#define rep(i, n)		forab (i, 0, (n) - 1)
#define For(i, n)		forab (i, 1, n)
#define rofba(i, a, b)	for (__typeof (b)i = (b) ; i >= a ; --i)
#define per(i, n)		rofba (i, 0, (n) - 1)
#define rof(i, n)		rofba (i, 1, n)
#define forstl(i, s)	for (__typeof ((s).end ()) i = (s).begin (); i != (s).end (); ++i)

#define __(args...) {dbg,args; cerr<<endl;}
struct debugger{template<typename T> debugger& operator , (const T& v){cerr<<v<<"\t"; return *this; }}dbg;
#define __1D(a,n) rep(i,n) { if(i) printf(" ") ; cout << a[i] ; }
#define __2D(a,r,c,f) forab(i,f,r-!f){forab(j,f,c-!f){if(j!=f)printf(" ");cout<<a[i][j];}cout<<endl;}

template<class A, class B> ostream &operator<<(ostream& o, const pair<A,B>& p){ return o<<"("<<p.ff<<", "<<p.ss<<")";} //Pair print
template<class T> ostream& operator<<(ostream& o, const vector<T>& v){ o<<"[";forstl(it,v)o<<*it<<", ";return o<<"]";} //Vector print
template<class T> ostream& operator<<(ostream& o, const set<T>& v){ o<<"[";forstl(it,v)o<<*it<<", ";return o<<"]";} //Set print
template<class T> inline void MAX(T &a , T b){ if (a < b ) a = b;}
template<class T> inline void MIN(T &a , T b){ if (a > b ) a = b;}

//Fast Reader
template<class T>inline bool read(T &x){int c=gc();int sgn=1;while(~c&&c<'0'||c>'9'){if(c=='-')sgn=-1;c=gc();}for(x=0;~c&&'0'<=c&&c<='9';c=gc())x=x*10+c-'0';x*=sgn;return ~c;}

//int dx[]={1,0,-1,0};int dy[]={0,1,0,-1}; //4 Direction
//int dx[]={1,1,0,-1,-1,-1,0,1};int dy[]={0,1,1,1,0,-1,-1,-1};//8 direction
//int dx[]={2,1,-1,-2,-2,-1,1,2};int dy[]={1,2,2,1,-1,-2,-2,-1};//Knight Direction
//int dx[]={2,1,-1,-2,-1,1};int dy[]={0,1,1,0,-1,-1}; //Hexagonal Direction

Long toInt(string s){ Long r = 0 ; istringstream sin(s); sin>>r; return r; }
string toString(Long number){ stringstream ss; ss << number; return ss.str(); }
vector<string>Split(string s ){ vector<string>ret; istringstream ss(s) ; string token ; while(ss >> token) ret.pb( token ) ; return ret; }
Long BigMod(Long  b,Long  p ,Long  m ){ Long  ret = 1 ;  while(p) { if(p&1) ret = (ret * b ) % m ; p >>= (1ll) , b = (b*b)%m ; } return ret ; }
string i2b(Long a) { string s ; do { s += (a&1) + '0' ,  a >>= 1; }while(a); reverse(all(s)); return s; }
Long GCD(Long a , Long b){ while(b) b ^=  a ^= b ^= a %= b ; return a; }

const int MX = 1e5 + 7 ;
const int INF = 1e8 + 7 ;



int main() {
#ifdef LOCAL
    FI ;
     FO ;
#endif

    int test = II ;
    while( test-- ) {
            string s ;
            cin >> s ;
            Long p = 0 , q = 0 ;
            int x = 0 ;
            for( ; s[x] != '/' ; x++ ) p = p*10 + s[x] - '0';
            for( x++ ; s[x] ; x++ ) q = q*10 + s[x] - '0';
            int Ans = 0 ;
            Long g = GCD(p,q);
            p /= g , q /= g ;
            if( q & 1 ) {
                p <<= 1ll ;
                q <<= 1ll ;
            }
            q >>= 1ll ;
            bool impossible = false ;
            while( true ){
                ++Ans;
                if( p < q ) {
                    if( q % 2 == 0 ) q >>= 1ll ;
                    else p <<= 1ll ;
                }else {
                    while( q % 2 == 0 ) q >>= 1ll ;
                    if( q == 1 ) break ;
                    else {
                        impossible = true ;
                        break ;
                    }
                }
            }
            CS ;
            if( impossible ) printf(" impossible\n");
            else cout << " " << Ans << endl;
    }
}
