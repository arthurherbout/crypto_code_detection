#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

int kases;
int r, c;
char s[100][100], vis[100][100], tmpvis[100][100];

int check_ok(int x, int y) {
    vis[x][y] = 1;
    memcpy(tmpvis, vis, sizeof vis);
    if (s[x][y] == '|') {
        for (int i = x - 1; i > 0 && s[i][y] != '#'; --i) {
            if (s[i][y] == '-' || s[i][y] == '|') return 0;
            tmpvis[i][y] = 1;
        }
        for (int i = x + 1; i <= r && s[i][y] != '#'; ++i) {
            if (s[i][y] == '-' || s[i][y] == '|') return 0;
            tmpvis[i][y] = 1;
        }
    } else {
        for (int i = y - 1; i > 0 && s[x][i] != '#'; --i) {
            if (s[x][i] == '-' || s[x][i] == '|') return 0;
            tmpvis[x][i] = 1;
        }
        for (int i = y + 1; i <= c && s[x][i] != '#'; ++i) {
            if (s[x][i] == '-' || s[x][i] == '|') return 0;
            tmpvis[x][i] = 1;
        }
    }
    memcpy(vis, tmpvis, sizeof vis);
    return 1;
}

int main() {
    #ifdef ULTMASTER
    freopen("a.in", "r", stdin);
    freopen("a.out", "w", stdout);
    #endif
    scanf("%d", &kases);
    for (int kase = 1; kase <= kases; ++kase) {
        scanf("%d%d", &r, &c);
        memset(vis, 0, sizeof vis);
        for (int i = 1; i <= r; ++i)
            scanf("%s", s[i] + 1);
        try {
            for (int i = 1; i <= r; ++i)
                for (int j = 1; j <= c; ++j) {
                    // printf("%d %d\n", i, j);
                    if (s[i][j] == '|') {
                        if (!check_ok(i, j)) {
                            s[i][j] = '-';
                            if (!check_ok(i, j))
                                throw 1;
                        }
                    } else if (s[i][j] == '-') {
                        if (!check_ok(i, j)) {
                            s[i][j] = '|';
                            if (!check_ok(i, j))
                                throw 2;
                        }
                    }
                }
            for (int i = 1; i <= r; ++i)
                for (int j = 1; j <= c; ++j)
                    if (s[i][j] == '.' && vis[i][j] == 0)
                        throw 3;
            printf("Case #%d: POSSIBLE\n", kase);
            for (int i = 1; i <= r; ++i)
                puts(s[i] + 1);
        } catch (int e) {
            // printf("%d\n", e);
            printf("Case #%d: IMPOSSIBLE\n", kase);
        }
        
    }
    return 0;
}
