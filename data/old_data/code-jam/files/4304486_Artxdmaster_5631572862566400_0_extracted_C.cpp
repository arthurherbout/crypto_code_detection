#include <stdio.h>
#include <algorithm>

using namespace std;

vector < int > graph[1100];
bool bff[1100][1100];
bool visited[1100];
int sol;

void dfs(int p, int u, int cont) {
    visited[u] = true;
    cont++;
    for (int v : graph[u]) {
        if (!visited[v] && (bff[u][p] || bff[u][v]))
            dfs(u, v, cont);
        else if (visited[v] && (bff[u][p] || bff[u][v])) sol = max(sol, cont);
    }
}

int main() {
    int t, n, x;
    scanf("%d", &t);

    for (int tc = 0; tc < t; tc++) {
        printf("Case #%d: ", tc+1);
        scanf("%d", &n);

        for (int i = 0; i < 1100; i++) {
            graph[i].clear();
            visited[i] = 0;
            for (int j = 0; j < 1100; j++)
                bff[i][j] = 0;
        }

        sol = 0;

        for (int i = 0; i < n; i++) {
            scanf("%d", &x);
            graph[i+1].push_back(x);
            graph[x].push_back(i+1);
            bff[i+1][x] = true;
        }

        for (int i = 1; i <= n; i++) {
            dfs(0, i, 0);
        }

        printf("%d\n", sol);
    }
    return 0;
}
