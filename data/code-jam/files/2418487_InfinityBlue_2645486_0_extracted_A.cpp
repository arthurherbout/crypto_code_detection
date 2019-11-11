#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <map>
using namespace std;
#define INF 0x73737373
#define EPS 1e-8
#define lson l, m, rt<<1
#define rson m+1, r, rt<<1|1
int E, R, N;
int vals[15], ret;
void dfs(int now, int energy, int val){
    if(now == N){
        ret = max(ret, val);
        return;
    }
    dfs(now + 1, min(E, energy + R), val);
    for(int i = 1; i <= energy; i++)
        dfs(now + 1, min(E, energy + R - i), val + i * vals[now]);
}
int main()
{
    int T;
    freopen("B-small-attempt0.in", "r", stdin);
    freopen("b.out", "w", stdout);
    scanf("%d", &T);
    for(int ca = 1; ca <= T; ca++)
    {
        ret = 0;
        scanf("%d%d%d", &E, &R, &N);
        for(int i = 0; i < N; i++)
        {
            scanf("%d", &vals[i]);
        }
        dfs(0, E, 0);
        printf("Case #%d: %d\n", ca, ret);
    }
	return 0;
}
