#include <bits/stdc++.h>

using namespace std;

#define ll long long

int t, ac, aj;

struct interv {
    ll c, d;
} intervals[220];

bool cmp(interv a, interv b) {
    return a.c < b.c;
}

int main() {
    scanf("%d", &t);
    for (int tc = 0; tc < t; tc++) {
        scanf("%d %d", &ac, &aj);
        for (int i = 0; i < ac + aj; i++)
            scanf("%lld %lld", &intervals[i].c, &intervals[i].d);

        sort(intervals, intervals+ac+aj, cmp);

        if (ac + aj == 1) printf("Case #%d: 2\n", tc+1);
        else if (ac == aj) printf("Case #%d: 2\n", tc+1);
        else {
            if (intervals[1].c - intervals[0].d >= 720 || 24*60 - intervals[1].d + intervals[0].c >= 720) printf("Case #%d: 2\n", tc+1);
            else printf("Case #%d: 4\n", tc+1);
        }
    }

    return 0;
}

