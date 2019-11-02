#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <vector>
using namespace std;

const int N = 20;

int n;
vector<int> edge[N+2];
bool vis[N+2][N+2];
int color[N+2];

int verify(int pre, int now, int nxt) {
    int t1 = now - pre;
    if (now < pre) t1 += n;
    t1 += nxt - now;
    if (nxt < now) t1 += n;
    return t1;
}

int dfs(int pre, int now, int st) {
    vis[pre][now] = true;
    vis[now][pre] = true;
    //cout << pre << "-->" << now << endl;//
    if (now == st) return 1;
    int nxt = -1, cur = 0;
    for (int i = 0; i < edge[now].size(); i++) {
        int tmp = verify(pre, now, edge[now][i]);
        //cout << pre << " " << now << " " << edge[now][i] << " = " << tmp << endl;//
        if (tmp >= n) continue;
        if (nxt == -1) nxt = edge[now][i];
        else if (tmp > cur) {
            nxt = edge[now][i];
            cur = tmp;
        }
    }
    return dfs(now, nxt, st) + 1;
}

bool yes[N+2];

int dbfs(int pre, int now, int st) {
    yes[ color[pre] ] = true;
    yes[ color[now] ] = true;
    vis[pre][now] = true;
    vis[now][pre] = true;
    //cout << pre << "-->" << now << endl;//
    if (now == st) return 1;
    int nxt = -1, cur = 0;
    for (int i = 0; i < edge[now].size(); i++) {
        int tmp = verify(pre, now, edge[now][i]);
        //cout << pre << " " << now << " " << edge[now][i] << " = " << tmp << endl;//
        if (tmp >= n) continue;
        if (nxt == -1) nxt = edge[now][i];
        else if (tmp > cur) {
            nxt = edge[now][i];
            cur = tmp;
        }
    }
    return dbfs(now, nxt, st) + 1;
}

bool check(int ans) {
        memset(vis, false, sizeof(vis));
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < edge[i].size(); j++) {
                if (vis[i][ edge[i][j] ]) continue;
                bool flag = false;
                for (int k = 0; k < edge[i].size(); k++) {
                    int tmp = verify(edge[i][k], i, edge[i][j]);
                    if (tmp < n) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) continue;
                memset(yes, false, sizeof(yes));
                dbfs(i, edge[i][j], i);
                int sz = 0;
                for (int i = 1; i <= ans; i++) {
                    if (yes[i]) ++sz;
                }
                if (sz < ans) return false;
                //cout << i << "," << edge[i][j] << " " << sz << endl;//
            }
        }
        return true;
}

bool ddfs(int now, int ans) {
    if (now > n) {
        if (check(ans)) return true;
        return false;
    }
    for (int i = 1; i <= ans; i++) {
        color[now] = i;
        if (ddfs(now+1, ans)) return true;
    }
    return false;
}

int main() {
    //freopen("D:\\TopCoder\\gcj2011\\R1\\C.in", "r", stdin);
    freopen("D:\\TopCoder\\gcj2011\\R1\\C.out", "w", stdout);
    int T;
    scanf("%d", &T);
    for (int ca = 1; ca <= T; ca++) {
        int m;
        scanf("%d %d", &n, &m);
        int st[m], ed[m];
        for (int i = 0; i < m; i++) {
            scanf("%d", &st[i]);
        }
        for (int i = 0; i < m; i++) {
            scanf("%d", &ed[i]);
        }
        for (int i = 1; i <= n; i++) {
            edge[i].clear();
            edge[i].push_back( (i) % n + 1);
            edge[i].push_back( (i-2+n+n) % n + 1);
        }
        for (int i = 0; i < m; i++) {
            edge[ st[i] ].push_back(ed[i]);
            edge[ ed[i] ].push_back(st[i]);
        }
        int ans = n;
        memset(vis, false, sizeof(vis));
        for (int i = 1; i <= n; i++) {
            for (int j = 0; j < edge[i].size(); j++) {
                if (vis[i][ edge[i][j] ]) continue;
                bool flag = false;
                for (int k = 0; k < edge[i].size(); k++) {
                    int tmp = verify(edge[i][k], i, edge[i][j]);
                    if (tmp < n) {
                        flag = true;
                        break;
                    }
                }
                if (!flag) continue;
                int sz = dfs(i, edge[i][j], i);
                if (sz < ans) ans = sz;
                //cout << i << "," << edge[i][j] << " " << sz << endl;//
            }
        }
        
        for (int i = ans; i >= 3; i--) {
            if (ddfs(1, i)) {
                ans = i;
                break;
            }
        }
        
        printf("Case #%d: %d\n", ca, ans);
        for (int i = 1; i <= n; i++) {
            if (i > 1) printf(" ");
            printf("%d", color[i]);
        }
        printf("\n");
    }
    //fclose(stdin);
    fclose(stdout);
    return 0;
}



