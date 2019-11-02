#include<bits/stdc++.h>
using namespace std;

#define ff first
#define ss second
#define all(data) data.begin(),data.end()

#define sig cout << "reached" << endl;
#define parr(data) {for(auto da: data) cout << da << " "; cout << endl;}
#define pmap(data) {for(auto da: data) cout << da.ff << " " << da.ss << endl; }
#define pmat(data) {for(auto dat: data) {for(auto da: dat) cout << da << " "	; cout << endl;} }
#define input freopen("i.in", "r", stdin);
#define output freopen("o.out", "w", stdout);
#define mset(mem, val) memset(mem, val, sizeof(mem));

#define forr(i,s,e) for(int i=s; i<e; i++)
#define forre(i,s,e) for(int i = s; i>=e; i--)

#define sd(x) {scanf("%d",&x);}
#define slld(x) {scanf("%lld",&x);}
#define sf(x) {scanf("%Lf",&x);}
#define sc(x) {scanf(" %c",&x);}

#define pd(x) {printf("%d\n",x);}
#define pd_(x) {printf("%d ",x);}
#define plld(x) {printf("%lld\n",x);}
#define plld_(x) {printf("%lld ",x);}
#define nl {printf("\n");}

typedef unsigned long long int llu; typedef long long int ll;
typedef pair<int, int> pii; typedef pair<ll, ll> pll;
typedef vector<int> vi;  typedef vector<vi> vvi;  typedef vector<ll> vl;    typedef vector<vl> vvl;
typedef vector<double> vd; typedef vector<vd> vvd;
typedef vector<pii> vpi; typedef vector<pll> vpl; typedef vector<vpi> vvpi; typedef vector<vpl> vvpl;
typedef vector<bool> vb; typedef vector<vb> vvb;  typedef vector<char> vc;  typedef vector<vector<char> > vvc;

int inf = 1000000005; ll llinf = 2000000000000000005LL;
ll mod = 1000000007; ll mod9 = 1000000009;
double pi = 3.1415926535897; double eps = 1e-15;
int dx[] = { 1, -1, 0, 0, 1, -1, 1, -1 }, dy[] = { 0, 0, 1, -1, 1, 1, -1, -1 };

vb isprime; vi primes;
void seive(int n, bool wantlist = true){ isprime.resize(n + 1, true); isprime[0] = isprime[1] = false; int sq = sqrt(n + 1); forr(i, 2, sq + 1){ if (isprime[i]){ for (int j = i*i; j <= n; j += i) isprime[j] = false; } } for (int i = 2; wantlist && i <= n; i++){ if (isprime[i]) primes.push_back(i); } }
template<class T>
inline T gcd(T a, T b){ while (b > 0){ a %= b; swap(a, b); } return a; }
template<class T>
inline T lcm(T a, T b){ return a*b / gcd(a, b); }
template<class T>
inline vector<T> operator+(vector<T>& a, vector<T>& b) { assert(a.size() == b.size()); int n = a.size(); vector<T> c(n); for (int i = 0; i < n; i++) c[i] = a[i] + b[i]; return c; }
int fastMax(int x, int y) { return (((y - x) >> (32 - 1))&(x^y)) ^ y; }
inline ll bexp(ll x, ll n, ll m = mod){ ll res = 1; x %= m; while (n){ if (n & 1) res = res * x % m; x = x * x % m; n >>= 1; } return res; }
inline bool ispalin(string& str){ int n = str.length(); for (int i = 0; i < n / 2; i++) if (str[i] != str[n - i - 1]) return false; return true; }

//******************************************************************************************

int main(){
    input;
   output;
    int tst; cin >> tst;
    forr(test, 1, tst + 1){
        int n, r, o, y, g, b, v;
        cin >> n >> r >> o >> y >> g >> b >> v;
        string ans;
        bool done = false;
        while(!done){
            done = true;
            int m = ans.length();
            if(r > 0){
                int pos = 0;
                while(pos < m && (ans[pos] == 'R' || ans[(pos + m -  1) % m] == 'R')) pos++;
                if(m == 0 || pos < m){
                    ans.insert(pos, "R");
                    r--; done = false;
                }
            }
            m = ans.length();
            if(b > 0){
                int pos = 0;
                while(pos < m && (ans[pos] == 'B' || ans[(pos + m -  1) % m] == 'B')) pos++;
                if(m == 0 || pos < m){
                    ans.insert(pos, "B");
                    b--; done = false;
                }
            }
            m = ans.length();
            if(y > 0){
                int pos = 0;
                while(pos < m && (ans[pos] == 'Y' || ans[(pos + m -  1) % m] == 'Y')) pos++;
                if(m == 0 || pos < m){
                    ans.insert(pos, "Y");
                    y--; done = false;
                }
            }
        }//cout << ans << endl;
        cout << "Case #" << test << ": ";
        if(int(ans.length()) != n) cout << "IMPOSSIBLE" << endl;
        else cout << ans << endl;
    }
}
