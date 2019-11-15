#include <iostream>
#include <bits/stdc++.h>

using namespace std;
#define mp(x,y) make_pair((x),(y))
#define pb(x) push_back(x)
#define sz(x) ((int)(x).size())
#define debug(x) (cerr << #x << " = " << (x) << endl)
template<class I> void debug2(I b, I e) { while (b != e) cerr << *b++ << " "; cerr << endl; }

typedef long long int ll;
typedef unsigned long long int ull;

int solve() {
    vector<int> d, h, m;
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        int td,th,tm;
        cin >> td >> th >> tm;
        d.pb(td);
        h.pb(th);
        m.pb(tm);
    }
    
    // assuming <= 2 hikers
    
    int t01, t02;
    int s1, s2;
    
    if (d.size() == 1) {
        if (h[0] == 1) {
            return 0;
        } else {
            t01 = m[0];
            s1 = 360 - d[0];
            
            t02 = m[0] + 1;
            s2 = 360 - d[0];
        }
    } else { // d.size() == 2
        t01 = m[0];
        s1 = 360 - d[0];
        
        t02 = m[1];
        s2 = 360 - d[1];
    }
    
    double t1 = s1 / 360.0 * t01;
    double t2 = s2 / 360.0 * t02;
    
    if (t1 + t01 <= t2 || t2 + t02 <= t1) {
        return 1;
    }
    
    return 0;
}

int main(){
    int tests;
    cin >> tests;
    for (int test = 1; test <= tests; test++) {
        printf("Case #%d: ", test);
        cout << solve() << endl;
    }
    
    return 0;
}
