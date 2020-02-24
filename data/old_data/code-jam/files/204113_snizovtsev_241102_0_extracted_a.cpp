#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

int solve()
{
    int n;
    scanf("%d", &n);
    int a[n];
    for (int i = 0; i < n; i++) {
        a[i] = 0;
        for (int j = 0; j < n; j++) {
            char c;
            scanf("\t%c", &c);
            if (c == '1') a[i] = j;
        }
    }

    int res = 0 ;
    for (int i = 0; i < n; i++) {
        int q;
        if (a[i] <= i) continue;
        for (q = i+1; q < n; q++)
            if (a[q] <= i) break;
        for (int j = q; j > i; j--) {
            swap(a[j], a[j-1]);
            res++;
        }
        assert(a[i] <= i);
    }
    return res;
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int i = 1; i <= T; i++)
        printf("Case #%d: %d\n", i, solve());
    return 0;
}
