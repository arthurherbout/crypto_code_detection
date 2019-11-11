#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

#define sz(a) ((int)((a).size()))
#define forn(i, n) for (int i = 0; i < (n); i++)
#define foreach(i, a) for (typeof((a).begin()) i = (a).begin(); i != (a).end(); ++i)
#define eprintf(...) {fprintf(stderr,__VA_ARGS__);fflush(stderr);}
typedef pair<int, int> ii;
typedef long long LL;

LL a[1<<20];

int main() {
    
    int tests;
    scanf("%d\n", &tests);
    for (int test = 1; test <= tests; test++) {
        printf("Case #%d: ", test);
        int L, n, c;
        LL t;
        scanf("%d%lld%d%d", &L, &t, &n, &c);
        LL sum = 0;
        forn(i, c) scanf("%lld", &a[i]);
        for (int i = c; i < n; i++) a[i] = a[i % c];
        forn(i, n) sum += 2 * a[i];
        if (sum <= t) {
            printf("%lld\n", sum);
            continue;
        }
        sum = 0;
        int start = -1;
        forn(i, n) {
            if (sum + 2 * a[i] > t) {
                start = i;
assert(!((sum + 2 * a[i] - t) % 2));
                a[i] = (sum + 2 * a[i] - t) / 2;
                break;
            }
            sum += 2 * a[i];
        }
// for (int i = start; i < n; i++) printf("%lld ", a[i]); printf("\n");
        sort(a + start, a + n);
        LL ans = t;
        for (int i = n - 1; i >= start; i--) {
            if (L > 0) {
                L--;
                ans += a[i];
            } else {
                ans += 2 * a[i];
            }
        }
        printf("%lld\n", ans);
    }
    
    return 0;
}
