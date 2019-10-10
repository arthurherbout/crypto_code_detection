#include <cstdio>
#include <string>
#include <sstream>
using namespace std;

long long gcd(long long a, long long b) {
    if (b == 0) return a;
    return gcd(b, a%b);
}

void solve() {
    long long a, b;
    long long G = 1LL<<40;

    scanf("%lld/%lld\n", &a, &b);
    long long g = gcd(a, b);
    a /= g;
    b /= g;

    if (G%b != 0) {
        printf("impossible\n");
        return;
    }

    int lena = -1;
    for (int i = 0; i < 43; i++) {
        if (a & (1LL<<i)) {
            lena = i;
        }
    }
    int lenb = -1;
    for (int i = 0; i < 43; i++) {
        if (b & (1LL<<i)) {
            lenb = i;
        }
    }
    stringstream ss;
    ss << lenb - lena;
    printf("%s\n", ss.str().c_str());
}

int main() {
    int T;
    scanf("%d\n", &T);
    for (int qq = 0; qq < T; qq++) {
        printf("Case #%d: ", qq + 1);
        solve();
    }
}
