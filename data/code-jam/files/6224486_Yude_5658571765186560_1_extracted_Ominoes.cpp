#include <bits/stdc++.h>
using namespace std;


int main() {
    int totaltc;
    int X, R, C;
    int tc = 1;
    scanf("%d", &totaltc);
    while (totaltc--) {
        scanf("%d%d%d", &X,&R,&C);
        printf("Case #%d: ", tc++);
        if ((R*C)%X != 0 || X >= 7 || min(R,C) < X-1) {
            printf("RICHARD\n");
        } else {
            printf("GABRIEL\n");
        }
    }
    return 0;
}
