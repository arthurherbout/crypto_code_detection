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
int val[10], sel[10];
int R, N, M, K;
bool dfs_check(int now, int val, int tar)
{
    if(now == N)
    {
        return val == tar;
    }
    if(dfs_check(now + 1, val, tar)) return true;
    if(dfs_check(now + 1, val * sel[now], tar)) return true;
    return false;
}
bool check()
{
    for(int i = 0; i < K; i++)
        if(!dfs_check(0, 1, val[i])) return false;
    return true;
}
bool dfs(int now)
{
    if(now == N)
    {
        return check();
    }
    for(int i = 2; i <= M; i++)
    {
        sel[now] = i;
        if(dfs(now + 1)) return true;
    }
}
int main()
{
    int T;
    freopen("C-small-1-attempt0.in", "r", stdin);
    freopen("c.out", "w", stdout);
    scanf("%d", &T);
    for(int ca = 1; ca <= T; ca++)
    {
        scanf("%d%d%d%d", &R, &N, &M, &K);
        printf("Case #%d:\n",ca);
        for(int i = 0; i < R; i++)
        {
            for(int j = 0; j < K; j++)
                scanf("%d", &val[j]);
            if(dfs(0))
            {
                for(int i = 0; i < N; i++)
                    printf("%d",sel[i]);
            } else {
                for(int i = 0; i < N; i++)
                    printf("2");
            }
            puts("");
        }
    }
	return 0;
}
