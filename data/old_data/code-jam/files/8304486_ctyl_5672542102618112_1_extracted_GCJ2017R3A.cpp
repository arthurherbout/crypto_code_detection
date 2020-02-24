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

ll ans = 0;
ll f[11];
string ss;
vector<int> vis;

int sum(string s){
    int ret = 0;
    rep(i, s.size()){
        ret += s[i] - '0';
    }
    return ret;
}

ll comb(int n, int r){
    return f[n] / f[r] / f[n - r];
}

bool google(string s){
    rep(i, s.size()){
        if(s[i] - '0' > s.size()) return false;
    }
    return true;
}
// 1000 -> 624
void dfs(string s){
    if(sum(s) > s.size()) return;
    string t = "";
    rep(i, (int)s.size() - sum(s)){
        t += '0';
    }
    ll cmb = 1;
    int a = (int)s.size();
    rep(i, s.size()){
        rep(j, s[i] - '0'){
            t += '0' + (i + 1);
        }
        cmb *= comb(a, s[i] - '0');
        a -= s[i] - '0';
    }
    
    //    do {
    //        if(t == ss) continue;
    //        ans++;
    ////        cout << t <<  endl;
    //        if(sum(t) <= s.size()){
    //            dfs(t);
    //        }
    //        else{
    //
    //        }
    //    } while( next_permutation(all(t)) );
    if(sum(t) <= s.size()){
        do {
            if(t == ss) continue;
            ans++;
            //        cout << t <<  endl;
            
            dfs(t);
            
        } while( next_permutation(all(t)) );
    }
    else{
        ans += cmb;
    }
    
}

int main() {
    cin.tie(0);
    ios::sync_with_stdio(0);
    f[0] = 1;
    rep(i, 1, 11){
        f[i] = f[i - 1] * i;
    }
    // source code
    
    int T;
    cin >> T;
    rep(t, T){
        ans = 1;
        
        cin >> ss;
        dfs(ss);
        
        
        
        cout << "Case #" << t + 1 << ": ";
        
        output(ans);
        
    }
    
    
    return 0;
}
