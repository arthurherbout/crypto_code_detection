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
long long vals[15000];
int main()
{
    int T;
    freopen("B-large.in", "r", stdin);
    freopen("bl.out", "w", stdout);
    scanf("%d", &T);
    for(int ca = 1; ca <= T; ca++)
    {
        long long E, R, N;
        long long ret = 0;
        scanf("%lld%lld%lld", &E, &R, &N);
        for(int i = 0; i < N; i++)
            scanf("%lld", &vals[i]);
        int pos = 0, energy = E;
        if(N == 1)
        {
            ret = vals[0] * E;
            printf("Case #%d: %lld\n", ca, ret);
            continue;
        }
        while(true)
        {
            int scope = max(1LL, E / R), index = -1;
            for(int i = 1; i <= scope && pos + i < N; i++)
                if(vals[pos + i] >= vals[pos]){
                    index = i;
                    break;
                }
            if(index == -1){
                ret += (energy) * vals[pos];
                energy = R;
                pos += 1;
            } else {
                ret += max(0LL, R * index - (E - energy)) * vals[pos];
                energy = min(E, energy + R * index);
                pos += index;
            }
            if(pos == N - 1){
                ret += energy * vals[pos];
                break;
            }
        }
        printf("Case #%d: %lld\n", ca, ret);
    }
	return 0;
}
