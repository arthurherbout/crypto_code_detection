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
int calArea(int r)
{
    return r * r - (r-1) * (r-1);
}
int main()
{
    int T;
    freopen("A-small-attempt0.in", "r", stdin);
    freopen("a.out", "w", stdout);
    scanf("%d", &T);
    for(int ca = 1; ca <= T; ca++)
    {
        int r, t, ret = 0;
        scanf("%d%d", &r, &t);
        r++;
        while(true)
        {
            t -= calArea(r);
            ret++;
            r+=2;
            if(t < calArea(r)) break;
        }
        printf("Case #%d: %d\n", ca, ret);
    }
	return 0;
}
