#include <bits/stdc++.h>
using namespace std;

const int SZ = 3333;
typedef long long ll;

struct pt {
    ll x, y;
    int id;
    bool operator < (const pt& o) const {
        if (x == o.x) return y < o.y;
        return x < o.x;
    }
} up[SZ], dn[SZ], inp[SZ];

int n, uct, dct, maxon[SZ];

ll prod(pt a1, pt a2, pt a3) {
   return (a1.x - a2.x) * (a3.y - a2.y) - (a1.y - a2.y) * (a3.x - a2.x);
}

void chull(vector<pt> dat) {
    sort(dat.begin(), dat.end());
    uct = dct = 0;
    up[uct++] = dat.front();
    dn[dct++] = dat.front();
    auto lv = dat.front(), rv = dat.back();

    for (int i = 1; i < dat.size(); ++i) {
        if (prod(lv, dat[i], rv) >= 0) {
            while (uct > 1 && prod(up[uct - 2], up[uct - 1], dat[i]) < 0) --uct;
            up[uct++] = dat[i];
        }

        if (prod(lv, dat[i], rv) <= 0) {
            while (dct > 1 && prod(dn[dct - 2], dn[dct - 1], dat[i]) > 0) --dct;
            dn[dct++] = dat[i];
        }
    }
    for (int i = 0; i < dct; ++i) {
        maxon[dn[i].id] = max(maxon[dn[i].id], int(dat.size()));
    }
    for (int i = 0; i < uct; ++i) {
        maxon[up[i].id] = max(maxon[up[i].id], int(dat.size()));
    }
}

void solve() {
    memset(maxon, 0, sizeof(maxon));
    scanf("%d", &n);

    for (int i = 0; i < n; ++i) {
        scanf("%lld%lld", &inp[i].x, &inp[i].y);
        inp[i].id = i;
    }
    for (int msk = 1; msk < (1 << n); ++msk) {
        vector<pt> dat;
        for (int i = 0; i < n; ++i) if (msk & (1 << i)) dat.push_back(inp[i]);
        chull(dat);
    }
    for (int i = 0; i < n; ++i) {
        printf("%d\n", n - maxon[i]);
    }
}


int main() {
    int T;
    scanf("%d", &T);
    for (int _ = 1; _ <= T; ++_) {
        printf("Case #%d: \n", _);
        solve();
    }
    return 0;
}


