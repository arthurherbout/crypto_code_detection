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

int a[10500];

int main() {
    
    int tests;
    scanf("%d\n", &tests);
    for (int test = 1; test <= tests; test++) {
        printf("Case #%d: ", test);
        int n, left, right;
        scanf("%d%d%d", &n, &left, &right);
        forn(i, n) scanf("%d", &a[i]);
        int res = -1;
        for (int ans = left; ans <= right; ans++) {
            bool ok = true;
            forn(i, n) {
                if ((ans % a[i]) && (a[i] % ans)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                res = ans;
                break;
            }
        }
        if (res < 0) printf("NO\n");
            else printf("%d\n", res);
    }
    
    return 0;
}
