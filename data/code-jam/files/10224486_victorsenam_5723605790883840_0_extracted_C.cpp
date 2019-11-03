#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long int ull;
typedef long long int ll;

#ifndef ONLINE_JUDGE
#define DEBUG(...) {fprintf(stderr, __VA_ARGS__);}
#else
#define DEBUG(...) {}
#endif

const int N = 16;
int uf[4*N], wf[4*N], us[4*N];
int mask;
int t;
int r, c;
int p[N];

int find (int i) {
    if (uf[i] == i) return i;
    return uf[i] = find(uf[i]);
}

void join(int i, int j) {
    if ((i = find(i)) == (j = find(j))) return;
    if (wf[i] < wf[j]) swap(i, j);
    wf[i] += wf[j];
    uf[j] = i;
}

int getPos (int i) {
    i--;
    if (i < c)
        return 4*i+1;
    i -= c;
    if (i < r)
        return 4*(c-1)+4*i*c+2;
    i -= r;
    if (i < c)
        return 4*(r*c-1)-4*i+3;
    i -= c;
    return 4*((r-1)*c)-4*i*c;
}

int main () {
    scanf("%d", &t);

    for (int cs = 1; cs <= t; cs++) {
        scanf("%d %d", &r, &c);       
        for (int i = 0; i < 2*(r+c); i++) {
            scanf("%d", &p[i]);
        }
        bool ok = 0;
        for (mask = 0; !ok && mask < (1<<(r*c)); mask++) {
            for (int i = 0; i < r*c; i++) {
                for (int j = 0; j < 4; j++) {
                    uf[4*i+j] = 4*i+j;
                    wf[4*i+j] = 1;
                    us[4*i+j] = 0;
                }
            }
            
            for (int i = 0; i < r*c; i++) {
                if (mask&(1<<i)) {
                    join(4*i, 4*i+1);
                    join(4*i+2, 4*i+3);
                } else {
                    join(4*i+1, 4*i+2);
                    join(4*i, 4*i+3);
                }

                int pr = i/c;
                int pc = i%c;
                if (pr < r-1)
                    join(4*i+3, 4*(i+c)+1);
                if (pr > 0)
                    join(4*i+1, 4*(i-c)+3);
                if (pc > 0)
                    join(4*i, 4*(i-1)+2);
                if (pc < c-1)
                    join(4*i+2, 4*(i+1));
            }

            ok = 1;
            for (int i = 0; ok && i < 2*(r+c); i+=2) {
                int a = find(getPos(p[i]));
                int b = find(getPos(p[i+1]));
                if (a != b) ok = 0;
                if (us[a]) ok = 0;
                us[a] = 1;
            }
        }
        mask--;
        printf("Case #%d:\n", cs);
        if (!ok)
            printf("IMPOSSIBLE\n");
        else {
            int j = 0;
            for (int i = 0; i < r*c; i++) {
                if (mask&(1<<i)) putchar('/');
                else putchar('\\');
                j++;
                if (j == c) {
                    putchar('\n');
                    j = 0;
                }
            }
        }
    }
}
