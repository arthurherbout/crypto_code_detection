#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

const double INF = 1e10;
int N, M, H;
int F[105][105], C[105][105];
double cost[105][105];
bool used[105][105];

double solve() {
    queue <pair <int, int> > Q;
    for (int i = 0; i < N; ++i) 
        for (int j = 0; j < M; ++j)
            if (!used[i][j]) {
                cost[i][j] = INF;
            } else {
                cost[i][j] = 0.0;
                Q.push(make_pair(i, j));
            }     
    while (!Q.empty()) {
        int r = Q.front().first, c = Q.front().second;
       // cout << r << " " << c << " " << cost[r][c] << endl;
        used[r][c] = false;
        Q.pop();
        double water_level = max(H - cost[r][c] * 10.0, 0.0);
        for (int dr = -1; dr <= 1; ++dr)
            for (int dc = -1; dc <= 1; ++dc)
                if (dr == 0 ^ dc == 0) {
                    int nr = r + dr, nc = c + dc;
                    if (nr < 0 || nc < 0 || nr >= N || nc >= M || C[nr][nc] - F[nr][nc] < 50 || C[nr][nc] - F[r][c] < 50 || C[r][c] - F[nr][nc] < 50)
                        continue;
                    double nextTime = cost[r][c];
                    if ((double)C[nr][nc] - water_level < 50.0) {
                        nextTime += (50.0 - C[nr][nc] + water_level) / 10.0;
                    }   
                    double next_level = max(H - nextTime * 10.0, 0.0);
                    //cout << nextTime << endl; 
                    int delta = (next_level >= (double)F[r][c] + 20.0) ? 1 : 10;
                    nextTime += delta;
                    if (nextTime < cost[nr][nc]) {
                        cost[nr][nc] = nextTime;
                        if (!used[nr][nc]) {
                            Q.push(make_pair(nr, nc));
                            used[nr][nc] = true;
                        }    
                    }      
                }    
    }    
    return cost[N - 1][M - 1];
}
    
void dfs(int r, int c) {
    used[r][c] = true;
    for (int dr = -1; dr <= 1; ++dr)
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 ^ dc == 0) {
                int nr = r + dr, nc = c + dc;
                if (nr < 0 || nc < 0 || nr >= N || nc >= M 
                || C[nr][nc] - max(F[r][c], H) < 50 || C[r][c] - max(F[nr][nc], H) < 50 || C[nr][nc] - max(F[nr][nc], H) < 50)
                    continue;
                if (!used[nr][nc]) dfs(nr, nc);
            }    
        }    
}
    
bool DFS() {
    memset(used, 0, sizeof(used));
    dfs(0, 0);
    return used[N - 1][M - 1];
}    


    
int main() {
    freopen("B-small-attempt0.in", "r", stdin);
    freopen("B-small-attempt0.out", "w", stdout);
    int T;
    scanf("%d", &T);
    for (int caseId = 1; caseId <= T; ++caseId) {
        scanf("%d %d %d", &H, &N, &M);
        for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) scanf("%d", &C[i][j]);
        for (int i = 0; i < N; ++i) for (int j = 0; j < M; ++j) scanf("%d", &F[i][j]);
        if (DFS()) {
            printf("Case #%d: 0.0\n", caseId);
        } else {
            double res = solve();
            printf("Case #%d: %.10lf\n", caseId, res);
        }        
        //system("pause");
    }    
   // system("pause");
    return 0;
}    
