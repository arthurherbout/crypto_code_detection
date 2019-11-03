#include<bits/stdc++.h>
#define ALL(X)        X.begin(),X.end()
#define FOR(I,A,B)    for(int (I) = (A); (I) <= (B); (I)++)
#define FORW(I,A,B)   for(int (I) = (A); (I) < (B);  (I)++)
#define FORD(I,A,B)   for(int (I) = (A); (I) >= (B); (I)--)
#define CLEAR(X)      memset(X,0,sizeof(X))
#define SIZE(X)       int(X.size())
#define CONTAINS(A,X) (A.find(X) != A.end())
#define PB            push_back
#define MP            make_pair
#define X             first
#define Y             second
using namespace std;
template<typename T, typename U> ostream& operator << (ostream& os, const pair<T, U> &_p) { return os << "(" << _p.X << "," << _p.Y << ")"; }
template<typename T> ostream& operator << (ostream &os, const vector<T>& _V) { bool f = true; os << "["; for(auto v: _V) { os << (f ? "" : ",") << v; f = false; } return os << "]"; }
template<typename T> ostream& operator << (ostream &os, const set<T>& _S) { bool f = true; os << "("; for(auto s: _S) { os << (f ? "" : ",") << s; f = false; } return os << ")"; }
template<typename T, typename U> ostream& operator << (ostream &os, const map<T, U>& _M) { return os << set<pair<T, U>>(ALL(_M)); }
typedef signed long long slong;
typedef long double ldouble;
typedef pair<int,int> pii;
const slong INF = 1000000100;
const ldouble EPS = 1e-9;

template<class TH> void _dbg(const char *sdbg, TH h){cerr<<sdbg<<"="<<h<<"\n";}
template<class TH, class... TA> void _dbg(const char *sdbg, TH h, TA... a) {
  while(*sdbg!=',')cerr<<*sdbg++;cerr<<"="<<h<<","; _dbg(sdbg+1, a...);
}

#ifdef LOCAL
  #define DBG(...) _dbg(#__VA_ARGS__, __VA_ARGS__)
#else
  #define DBG(...) ;
  #define cerr if(0)cout
#endif

const int N = 1000000000;

/*string goo(string g) {
    int L = SIZE(g);
    vector<int> h(L+1,0);
    for(char c : g) {
        h[c-'0']++;
    }
    string ret = "";
    FOR(i,1,L) ret.PB('0'+h[i]);
    return ret;
}*/

void solve(int num)
{
    string g;
    cin >> g;

    set<string> vis;
    queue<string> q;
    q.push(g);
    vis.insert(g);

    while(q.size()) {
        string cur = q.front();
        q.pop();
        //if(vis.count(cur)) continue;
        string up = "";
        int L = SIZE(cur);
        int sum = 0;
        FORW(i,0,L) sum += cur[i] - '0';
        int zeros = L - sum;

        if(sum > L) {
            continue;
        }

        FORW(i,0,zeros) up.PB('0');
        FORW(i,0,L) {
            int ile = cur[i] - '0';
            FORW(j,0,ile) up.PB('1'+i);
        }
        //DBG(cur,up);

        vector<string> perms;
        while(42) {
            if(!vis.count(up)) {
                q.push(up);
                vis.insert(up);
                //perms.PB(up);
            }
            if(!next_permutation(ALL(up))) break;
        }
        //DBG(cur,up,perms);
    }

    cout << "Case #" << num << ": " << SIZE(vis) << "\n";

}

int main()
{
    ios_base::sync_with_stdio(0);
    int t;
    cin >> t;
    for(int i=1; i <= t; i++)
    {
        solve(i);
    }
}
