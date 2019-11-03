#include <bits/stdc++.h>
using namespace std;

int totaltc;
int b, n;
int M[1001];

long long round(long long r) {
    long long ret = 0;
    for (int i = 1; i <= b; i++) {
        ret += r/M[i];
    }
    return ret;
}

int main() {
    int tc = 1;
    scanf("%d", &totaltc);
    while (totaltc--) {
        scanf("%d %d", &b, &n);
            //printf("n %d\n", n);
        for (int i = 1; i <= b; i++) {
            scanf("%d", &M[i]);
        }
        printf("Case #%d: ", tc++);
        if (n <= b) {
            printf("%d\n", n);
        } else {
            long long left, right, mid;
            left = 0; right = 100000000000000000L;
            long long NL = (long long)n - (long long)b;
            while (left != right) {
                mid = left + (right-left+1)/2;
                long long rr = round(mid);
                //printf("mid %lld round(mid) %lld NL %lld\n", mid, rr, NL);
                if (rr >= NL) {
                    right = mid-1;
                } else {
                    left = mid;
                }
            }
            long long rr = round(left);
            while (round(right) >= NL) left--;
            while (round(left) < NL) left++;
            //printf("good round = %d\n", left);
            NL -= round(left-1);
            int ans = 1;
            for (; ans <= b && NL > 0; ans++) {
                if (left % M[ans] == 0) NL--;
                if (NL == 0) break;
            }
            printf("%d\n", ans);
        }
    }
    return 0;
}
