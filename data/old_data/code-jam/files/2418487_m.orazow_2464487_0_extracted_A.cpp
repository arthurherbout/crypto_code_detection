#include <iostream>
#include <cstdio>

using namespace std;

int main() {
    int T;
    long long r;
    long long t;
    scanf ("%d\n", &T);
    for (int tt = 1; tt <= T; tt++) {
        printf("Case #%d: ", tt);
        scanf("%lld %lld\n", &r, &t);
        unsigned long long four = ((r << 1) + 1);
        unsigned long long res = 0;
        while (1) {
            if (t <= 0) break;
            t -= four;
            if (t >= 0) res++;
            four += 4;
        }
        printf("%llu\n", res);
    }
    return 0;
}
