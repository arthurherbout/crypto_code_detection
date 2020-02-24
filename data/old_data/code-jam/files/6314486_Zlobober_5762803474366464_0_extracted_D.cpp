#include <cstdio>
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

typedef long long llong;

struct vt {
    llong x, y, z;
    vt(llong _x, llong _y, llong _z) {
        x = _x, y = _y, z = _z;
    }
    bool iszero() {
        return x == 0 && y == 0 && z == 0;
    }
    friend vt operator +(vt a, vt b) {
        return vt(a.x + b.x, a.y + b.y, a.z + b.z);
    }
    friend vt operator -(vt a, vt b) {
        return vt(a.x - b.x, a.y - b.y, a.z - b.z);
    }
    friend vt operator ^(vt a, vt b) {
        return vt(a.y * b.z - b.y * a.z, a.z * b.x - b.z * a.x, a.x * b.y - b.x * a.y);
    }
    friend llong operator *(vt a, vt b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    vt() {}
};

llong det(vt a, vt b, vt c) {
    return (a ^ b) * c;
}

int sgn(llong x) {
    return (x > 0) - (x < 0);
}

tuple<int, int, int> sgn3(vt v) {
    return make_tuple(sgn(v.x), sgn(v.y), sgn(v.z));
}

void solve(int cs) {
    int n;
    scanf("%d", &n);
    vector<vt> P;
    for (int i = 0; i < n; i++) {
        llong x, y, z;
        scanf("%lld %lld %lld", &x, &y, &z);
        P.emplace_back(x, y, z);
    }
    bool ans = false;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            for (int z = 0; z < j; z++) {
                for (int t = 0; t < z; t++) {
                    vt a = P[i];
                    vt b = P[j];
                    vt c = P[z];
                    vt d = P[t];
                    int sabc = sgn(det(a, b, c));
                    int sbad = sgn(det(b, a, d));
                    int scbd = sgn(det(c, b, d));
                    int sacd = sgn(det(a, c, d));
                    if (sabc == 0 || sbad == 0 || scbd == 0 || sacd == 0) {
                        continue;
                    }
                    if (sabc == sbad && sabc == scbd && sabc == sacd) {
                        ans = true;
                    }
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            for (int z = 0; z < j; z++) {
                vt a = P[i];
                vt b = P[j];
                vt c = P[z];
                if (det(a, b, c) != 0) {
                    continue;
                }
                vt vab = a ^ b;
                vt vbc = b ^ c;
                vt vca = c ^ a;
                vt v = (a - c) ^ (b - c);
                auto sab = sgn3(vab);
                auto sbc = sgn3(vbc);
                auto sca = sgn3(vca);
                auto sv = sgn3(v);
                static const tuple<int, int, int> zeroes(0, 0, 0);
                if (sab == zeroes || sbc == zeroes || sca == zeroes || sv == zeroes) {
                    continue;
                }
                if (sab == sv && sbc == sv && sca == sv) {
                    ans = true;
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            vt a = P[i];
            vt b = P[j];
            if ((a ^ b).iszero() && a * b < 0) {
                ans = true;
            }
        }
    }
    printf("Case #%d: %s\n", cs, ans ? "YES" : "NO");
}

int main(int argc, char* argv[]) {
    int T;
    scanf("%d", &T);
    for (int i = 0; i < T; i++) {
        solve(i + 1);
        fprintf(stderr, "%d\n", i);
    }
}
