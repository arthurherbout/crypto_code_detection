/**********************************************************************
Author: littlekid@whu_Dolphin
Created Time:  Sat 26 Jul 2008 09:25:52 AM CST
Modified Time: Sat 26 Jul 2008 09:47:27 AM CST
File Name: 
Description: 
**********************************************************************/
#include <iostream>
using namespace std;

#define out(x) printf("%s: %lld\n", #x, (long long)(x))
const int maxint=0x7FFFFFFF;
template <class T> void get_max(T& a, const T &b) {b > a? a = b:1;}
template <class T> void get_min(T& a, const T &b) {b < a? a = b:1;}

const int MAXN = 111;///

int n, m;
int ans;
int cnt;
int a[11];
int map[MAXN][11][2];
bool vis[MAXN];

void get_input()
{
    scanf("%d %d", &n, &m);
    int t, x, y;
    memset(map, 0, sizeof(map));
    for (int ix = 1; ix <= m; ++ ix)
    {
        scanf("%d", &t);
        for (int i = 1; i <= t; ++ i)
        {
            scanf("%d %d", &x, &y);
            map[ix][x][0] = 1;
            map[ix][x][1] = y;
        }
    }
}

void print_ans(int ca)
{
    printf("Case #%d:", ca);
    if (ans < 0)
    {
        printf(" IMPOSSIBLE\n");
        return ;
    }
    for (int ix = 1; ix <= n; ++ ix)
    {
        printf(" %d", a[ix]);///
    }
    printf("\n");
}

bool satisfy(int st)
{
    for (int ix = 1; ix <= n; ++ ix)
    {
        a[ix] = st%2;
        st /= 2;
    }
    memset(vis, false, sizeof(vis));
    cnt = 0;
    for (int ix = 1; ix <= m; ++ ix)
    {
        for (int i = 1; i <= n; ++ i)
        {
            if (map[ix][i][0] == 1 && a[i] == map[ix][i][1])
            {
                ++ cnt;
                break;
            }
        }
    }
    return (cnt == m);
}

void solve()
{
    int tot = (1<<n)-1;
    ans = -1;
    for (int ix = 0; ix <= tot; ++ ix)
    {
        if (satisfy(ix)) 
        {
            ans = ix;
            break;
        }
    }
}


int main() {
    int T; scanf("%d", &T);
    freopen("b_1.out", "w", stdout);
    for (int ca = 1; ca <= T; ++ ca)
    {
        get_input();
        solve();
        print_ans(ca);
    } 
    return 0;
}

