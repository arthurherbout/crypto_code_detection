#include <iostream>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <string>
#include <set>
using namespace std;

int ans, t, n, l;
set<long long> b;
string tmp;
long long la[200], lb[200];

long long cal() {
    long long ret = 0;
    for (int i = 0; i < l; i++)
        ret = ret * 2 + (tmp[i] - '0');
    return ret;
}
int solve(long long x) {
    int ret = 0;
    while (x > 0) {
        if (x % 2)
            ret++;
        x /= 2;
    }
    return ret;
}

int main() {
    freopen("ta.in", "r", stdin);
    freopen("ans1.txt", "w", stdout);
    scanf("%d", &t);
    for (int z = 1; z <= t; z++) {
        b.clear();
        scanf("%d%d", &n, &l);
        for (int i = 0; i < n; i++) {
            cin >> tmp;
            la[i] = cal();
        }
        for (int i = 0; i < n; i++) {
            cin >> tmp;
            lb[i] = cal();
            b.insert(lb[i]);
        }
        ans = 1000;
        for (int i = 0; i < n; i++) {
            long long x = la[i] ^ lb[0];
            int tmpa = solve(x);
            set<long long> sb(b);
            bool flag = true;
            for (int j = 0; j < n; j++) {
                if (sb.find(la[j] ^ x) != sb.end()) {
                    sb.erase(la[j] ^ x);
                } else {
                    flag = false;
                    break;
                }
            }
            if (flag)
                ans = min(ans, tmpa);
        }
        if (ans <= l) {
            printf("Case #%d: %d\n", z, ans);
        } else {
            printf("Case #%d: NOT POSSIBLE\n", z);
        }
    }
    return 0;
}
