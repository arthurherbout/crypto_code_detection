#include <cstdio>
#include <cstring>
using namespace std;

int bd[201][201];
int N;
int dx[] = {-1,0,1,0};
int dy[] = {0,1,0,-1};

bool inside(int x, int y) {
    if (bd[x][y] == 1) return 0;
    int ct = 0;
    for (int i = y+1; i <= 200; i++) {
        if (bd[x][i] == 1 && bd[x+1][i] == 1) {
            ct++;
        }
    }
    return (ct % 2 == 1);
}
bool ew(int x, int y) {
    bool ok = 0;
    for (int i = y - 1; i >= 0; i--) if (bd[x][i] == 1) ok = 1;
    if (!ok) return 0; ok = 0;
    for (int i = y + 1; i <= 200; i++) if (bd[x][i] == 1) ok = 1;
    return ok;
}
bool ns(int x, int y) {
    bool ok = 0;
    for (int i = x - 1; i >= 0; i--) if (bd[i][y] == 1) ok = 1;
    if (!ok) return 0; ok = 0;
    for (int i = x + 1; i <= 200; i++) if (bd[i][y] == 1) ok = 1;
    return ok;
}

int main() {
    scanf("%d", &N);
    for (int it = 1; it <= N; it++) {
        memset(bd,0,sizeof(bd));
        int L; scanf("%d", &L);

        int x=100,y=100,d=0;
        for (int i = 0; i < L; i++) {
            char S[50];
            int T;
            scanf("%s %d", S, &T);

            for (int j = 0; j < T; j++) {
                for (int k = 0; S[k]; k++) {
                    if (S[k] == 'F') {
                        bd[x][y] = 1;
                        x += dx[d]; y += dy[d];
                    }
                    else if (S[k] == 'L') d = (d + 3) % 4;
                    else if (S[k] == 'R') d = (d + 1) % 4;
                }
            }
        }

        for (int i = 0; i <= 200; i++) {
            for (int j = 0; j <= 200; j++) {
                if (inside(i,j)) bd[i][j] = 2;
                else if (!inside(i,j) && bd[i][j] != 1) {
                    // outside and not boundary
                    if (ns(i,j) || ew(i,j)) {
                        bd[i][j] = 5;
                    }
                }
            }            
        }

        //        for (int i = 95; i <= 105; i++, printf("\n"))
        //            for (int j = 95; j <= 105; j++)
        //                printf("%d ", bd[i][j]);

        int ret = 0;
        for (int i = 0; i < 200; i++)
            for (int j = 0; j < 200; j++) {
                int zero=0,one=0,two=0,five=0;
                for (int k = 0; k < 2; k++)
                    for (int m = 0; m < 2; m++) {
                        if (bd[i+k][j+m] == 0) zero++;
                        if (bd[i+k][j+m] == 1) one++;
                        if (bd[i+k][j+m] == 2) two++;
                        if (bd[i+k][j+m] == 5) five++;
                    }
                if (zero == 0 && two == 0 && five > 0) {
                    ret++;
                }
            }
        printf("Case #%d: %d\n", it, ret);
    }
}
