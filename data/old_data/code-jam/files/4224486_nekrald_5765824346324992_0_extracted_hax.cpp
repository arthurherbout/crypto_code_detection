#include <bits/stdc++.h>
using namespace std;

const int SZ = 1111;
int b, n, m[SZ];

int imitate() {
   vector<int> loads(b, 0);
   int lid = 0, ld = 0;
   for (int jb = 0; jb < n; ++jb) {
       lid = 0; ld = loads[0];

       for (int i = 0; i < b; ++i) {
           if (loads[i] < ld) {
               ld = loads[i];
               lid = i;
           }
       }
       loads[lid] += m[lid];
   }
   return lid;
}

void solve() {
    scanf("%d%d", &b, &n);
    for (int i = 0; i < b; ++i) scanf("%d", m + i);
    int lcm = 1;
    for (int i = 0; i < b; ++i) {
        int d = __gcd(lcm, m[i]);
        lcm *= m[i];
        lcm /= d;
    }
    int miss = 0;
    for (int i = 0; i < b; ++i) miss += lcm / m[i];
    n %= miss;
    if (n == 0) n += miss;
    printf("%d\n", imitate() + 1);
}


int main() {
    int T; scanf("%d", &T);
    for (int _ = 1; _ <= T; ++_) {
        printf("Case #%d: ", _);
        solve();
    }
}
