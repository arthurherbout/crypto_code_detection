#include <iostream>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <set>
#include <algorithm>
using namespace std;


int a[10010];

int main() {
    freopen("a.in","r", stdin);
    freopen("ans.txt", "w", stdout);

    int t, x, n;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++) {
        scanf("%d%d", &n, &x);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        sort(a, a + n);

        multiset<int> st;
        multiset<int>::iterator p;
        int ans = n;
        for (int i = n - 1; i >= 0; i--) {
            if (!st.empty()) {
                p = st.end();
                p--;
                if (*p >= a[i]) {
                    ans--;
                    st.erase(p);
                    continue;
                }
            }
            if (x > a[i]) {
                st.insert(x - a[i]);
            }
        }
        printf("Case #%d: %d\n", z, ans);
    }
    return 0;
}
