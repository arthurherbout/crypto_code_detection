#include <bits/stdc++.h>

#define FO(i,a,b) for (int i = (a); i < (b); i++)
#define sz(v) int(v.size())

using namespace std;

typedef long long ll;

ll gv(map<ll,ll> &f, ll v) {
    if (f.count(v)) return f[v];
    else return 0;
}

bool good;
map<ll,ll> go(const map<ll,ll> &f, ll d) {
    map<ll,ll> a;
    good = true;
    if (d == 0) {
        for (auto p : f) {
            if (p.second % 2 == 1) {
                good = false;
                return a;
            }
            a[p.first] = p.second/2;
        }
    } else if (d > 0) {
        for (auto p : f) {
            a[p.first] = p.second-gv(a,p.first-d);
            if (a[p.first] < 0) {
                good = false;
                return a;
            }
        }
    } else {
        for (auto it = f.rbegin(); it != f.rend(); it++) {
            auto p = *it;
            a[p.first] = p.second-gv(a,p.first-d);
            if (a[p.first] < 0) {
                good = false;
                return a;
            }
        }
    }
    if (a[0] <= 0) {
        good = false;
        return a;
    }
    return a;
}

int main() {
    int T; scanf("%d", &T);
    FO(Z,1,T+1) {
        int p; scanf("%d", &p);
        vector<ll> e(p);
        map<ll,ll> f;
        FO(i,0,p) scanf("%lld", &e[i]);
        FO(i,0,p) {
            ll fr; scanf("%lld", &fr);
            f[e[i]] = fr;
        }
        vector<ll> res;
        for (ll x : e) {
            while (1) {
                //printf("x=%lld\n", x);
                map<ll,ll> nf = go(f,x);
                if (good) {
                    //printf("ALLG?\n");
                    f = nf;
                    res.push_back(x);
                } else break;
            }
        }
        printf("Case #%d:", Z);
        for (ll x : res) printf(" %lld", x);
        printf("\n");
        fprintf(stderr, "DONE %d\n", Z);
    }
}

