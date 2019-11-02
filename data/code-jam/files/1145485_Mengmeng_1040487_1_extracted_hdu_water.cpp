#include <iostream>
#include <cstdio>
using namespace std;
const int MAX = 50010;

inline int gcd(int a, int b) {
    return b ? gcd(b, a % b) : a;
}

int main() {
    freopen("A-large.in", "r", stdin);
    freopen("out2.txt", "w", stdout);

    int pd, pg;
    int T, t;
    int flag;
    long long n;

    //b?gcd(b,a%b);

    scanf("%d", &T);
    for (int t = 1; t <= T; t++) {
        cin >> n;
        scanf("%d%d", &pd, &pg);

        if ((pg == 0 && pd != 0) || (pg == 100 && pd != 100)) {
            flag = 1;
        } else {
            int tttt = gcd(pd, 100);

            if (100 / tttt <= n) {
                flag = 2;
            } else {
                flag = 1;
            }
        }

        if (flag == 1)
            printf("Case #%d: Broken\n", t);
        else
            printf("Case #%d: Possible\n", t);
    }
    return 0;
}