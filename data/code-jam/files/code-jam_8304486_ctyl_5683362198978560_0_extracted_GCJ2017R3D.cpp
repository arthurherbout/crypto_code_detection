#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cmath>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <sstream>
#include <string>
#define _repargs(_1,_2,_3,name,...) name
#define _rep(i,n) repi(i,0,n)
#define repi(i,a,b) for(int i=(int)(a);i<(int)(b);++i)
#define rep(...) _repargs(__VA_ARGS__,repi,_rep,)(__VA_ARGS__)
#define all(x) (x).begin(),(x).end()
#define mod 1000000007
#define inf 2000000007
#define mp make_pair
#define pb push_back
typedef long long ll;
using namespace std;
template <typename T>
inline void output(T a, int p = 0) {
    if(p) cout << fixed << setprecision(p)  << a << "\n";
    else cout << a << "\n";
}
// end of template

int main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    // source code
    
    int T;
    cin >> T;
    rep(t, T){
        int R, C, N;
        ll D;
        cin >> R >> C >> N >> D;
        vector<vector<ll>> A(R, vector<ll>(C, -1));
        vector<pair<pair<int, int>, int>> L(N);
        rep(i, N){
            int r, c, b;
            cin >> r >> c >> b;
            r--, c--;
            A[r][c] = b;
            L[i] = mp(mp(r, c), b);
        }
        
        bool ok = true;
        
        rep(i, N) rep(j, i + 1, N){
            int dist = abs(L[i].first.first - L[j].first.first) + abs(L[i].first.second - L[j].first.second);
            ll margin = D * dist;
            if(abs(L[i].second - L[j].second) > margin) ok = false;
        }
        
        rep(r, R) rep(c, C){
            if(A[r][c] > 0) continue;
            ll mi = -(1LL << 60), ma = 1LL << 60;
            rep(i, N){
                int dist = abs(r - L[i].first.first) + abs(c - L[i].first.second);
                ll margin = D * dist;
                mi = max(mi, L[i].second - margin);
                ma = min(ma, L[i].second + margin);
            }
//            cout <<  r << "," << c << "," << mi << "," << ma << endl;
            if(mi > ma) ok = false;
            A[r][c] = ma;
        }
        
        
        
        cout << "Case #" << t + 1 << ": ";
        if(ok){
            ll sum = 0;
            rep(r, R) rep(c, C){
                sum += A[r][c];
                sum %= mod;
            }
            output(sum);
        }
        else{
            output("IMPOSSIBLE");
        }
        
        
    }
    
    return 0;
}
