#include <bits/stdc++.h>

using namespace std;

#define ll long long

int t, ac, aj;

struct interv {
    ll c, d;
    bool cam;
} intervals[220];

bool cmp(interv a, interv b) {
    return a.c < b.c;
}

int dp[1500][1500][3][2], tim[1500];

int solve(int id, int q, int l, int f) {
    if (dp[id][q][l][f] != -1) return dp[id][q][l][f];

    if (id == 1440) {
        if (q == 720) return l != f;
        return 1000000000;
    }

    int ret = 1000000000;
    if (tim[id] == 0) ret = min(solve(id+1, q+1, 1, f == 0? 1 : f) + (l == 2), solve(id+1, q, 2, f == 0? 2 : f) + (l == 1));
    else if (tim[id] == 1) ret = solve(id+1, q, 2, f == 0? 2 : f) + (l == 1);
    else ret = solve(id+1, q+1, 1, f == 0? 1 : f) + (l == 2);

    return dp[id][q][l][f] = ret;
}

int main() {
    scanf("%d", &t);
    for (int tc = 0; tc < t; tc++) {
        scanf("%d %d", &ac, &aj);
        for (int i = 0; i < ac + aj; i++) {
            scanf("%lld %lld", &intervals[i].c, &intervals[i].d);
            intervals[i].cam = (i < ac);
        }

        sort(intervals, intervals+ac+aj, cmp);

        if (ac + aj == 1) printf("Case #%d: 2\n", tc+1);
        else if (ac == 1 && aj == 1) printf("Case #%d: 2\n", tc+1);
        else if (ac + aj == 2) {
            if (intervals[1].c - intervals[0].d >= 720 || 24*60 - intervals[1].d + intervals[0].c >= 720) printf("Case #%d: 2\n", tc+1);
            else printf("Case #%d: 4\n", tc+1);
        }
        else {
            for (int i = 0; i < ac+aj; i++)
                for (int j = intervals[i].c; j < intervals[i].d; j++) {
                    if (intervals[i].cam) tim[j] = 1;
                    else tim[j] = 2;
                }

            for (int i = 0; i < 1500; i++)
                for (int j = 0; j < 1500; j++)
                    for (int k = 0; k < 3; k++)
                        dp[i][j][k][0] = dp[i][j][k][1] = -1;

            int ans = solve(0, 0, 0, 0);
            if (ans > 1000000000) ans -= 1000000000;

            printf("Case #%d: %d\n", tc+1, ans);
        }
    }

    return 0;
}

