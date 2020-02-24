#include<stdio.h>
#include<string.h>
#include<map>
typedef long long LL;
int R, C, a[10][10], ans;
int dx[] = {-1, 1, 0, 0}, dy[] = {0, 0, -1, 1};
std::map<LL, int> m;
LL hash(int c) {
    LL h = 0;
    for(int i = c;;) {
        for(int j = 1; j <= R; j ++) {
            h = h * 3 + a[j][i];
        }
        ++ i;
        if(i > C) {
            i = 1;
        }
        if(i == c) {
            break;
        }
    }
    return h;
}
void cal() {
    LL h = hash(1);
    if(m[h] != 0) {
        return ;
    }
    ++ ans;
    for(int i = 1; i <= C; i ++) {
        h = hash(i);
        m[h] = 1;
    }
}
bool inside(int r) {
    return r >= 1 && r <= R;
}
bool check() {
    for(int r = 1; r <= R; r ++) {
        for(int c = 1; c <= C; c ++) {
            if(a[r][c] == 0) {
                continue;
            }
            int cnt = 0, zero = 0;
            for(int i = 0; i < 4; i ++) {
                int x = r + dx[i], y = c + dy[i];
                if(inside(x)) {
                    if(a[x][y] == 0) {
                        ++ zero;
                    } else if(a[x][y] == a[r][c]) {
                        ++ cnt;
                    }
                }
            }
            if(zero == 0) {
                if(cnt != a[r][c]) {
                    return false;
                }
            } else {
                if(cnt > a[r][c]) {
                    return false;
                }
            }
        }
    }
    return true;
}
void dfs(int r, int c) {
    if(c > C) {
        cal();
        return ;
    }
    for(int i = 1; i <= 3; i ++) {
        a[r][c] = i;
        if(c == 1) {
            a[r][C + 1] = i;
        }
        if(c == C) {
            a[r][0] = i;
        }
        if(check()) {
            if(r == R) {
                dfs(1, c + 1);
            } else {
                dfs(r + 1, c);
            }
        }
        a[r][c] = 0;
        if(c == 1) {
            a[r][C + 1] = 0;
        }
        if(c == C) {
            a[r][0] = 0;
        }
    }
}
int main() {
    //freopen("input.txt", "rb", stdin);
    freopen("D-small-attempt0.in", "rb", stdin);
    freopen("output.txt", "wb", stdout);
    int t;
    scanf("%d", &t);
    for(int tt= 1; tt <= t; tt ++) {
        scanf("%d%d", &R, &C);
        ans = 0;
        memset(a, 0, sizeof(a));
        m.clear();
        dfs(1, 1);
        printf("Case #%d: %d\n", tt, ans);
    }
    return 0;
}
