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
        if (X <= 2) {
            if (R % X == 0 || C % X == 0) {
                printf("GABRIEL\n");
            } else {
                printf("RICHARD\n");
            }
        } else if (X == 3) {
            if ((R*C)%X == 0 && R >= 2 && C >= 2 && R*C >= 6) {
                printf("GABRIEL\n");
            } else {
                printf("RICHARD\n");
            }
        } else if (X == 4) {
            if ((R*C)%X == 0 && R >= 3 && C >= 3 && R*C >= 12) {
                printf("GABRIEL\n");
            } else {
                printf("RICHARD\n");
            }
        } else if (X == 5) {
            if ((R*C)%X == 0) {

            } else {
                printf("RICHARD\n");
            }
        } else if (X == 6) {
            if ((R*C)%X == 0) {

            } else {
                printf("RICHARD\n");
            }
        } else {
            printf("RICHARD\n");
        }
    }
    return 0;
}
