#include <bits/stdc++.h>
using namespace std;
#define mp(x,y) make_pair((x),(y))
#define pb(x) push_back(x)
#define sz(x) ((int)(x).size())
#define debug(x) (cerr << #x << " = " << (x) << endl)
template<class I> void debug2(I b, I e) { while (b != e) cerr << *b++ << " "; cerr << endl; }

double solve() {
    int c, d, v;
    
    cin >> c >> d >> v;
    int a[5];
    for (int i=  0; i < d;i++){
        cin >> a[i];
    }
    sort(a, a + d);
    
    bool u[30 + 1];
    memset(u,false,sizeof(u));
    u[0] = 1;
    for (int i = 0; i < d; i++) {
        for (int j = v - a[i]; j >= 0; j--) {
            if (u[j]) u[j + a[i]] = true;
        }
    }
    
    int extra = 0;
    for (int i = 1; i <= v; i++) {
        if (!u[i]) {
            //cout << "Need coin " << i << endl;
            extra++;
            for (int j = v - i; j >= 0; j--) {
                if (u[j]) u[j + i] = true;
            }
        }
    }
    
    return extra;
}

int main() {
    //freopen("C.in", "r", stdin);
    
    int tests;
    cin >> tests;
    for (int test = 1; test <= tests; test++) {
        printf("Case #%d: ", test);
        cout << solve() << endl;
//        break;
    }
    
    return 0;
}
