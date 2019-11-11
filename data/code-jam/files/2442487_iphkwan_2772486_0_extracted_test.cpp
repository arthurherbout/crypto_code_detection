#include <iostream>
#include <cstdio>
#include <string.h>
#include <algorithm>
#include <stack>
using namespace std;
#define MOD 1000002013
int m;
long long n;
struct Line
{
    int inout;
    long long pos, p;
}L[2010];

struct Node
{
    int pos, p;
    Node(int a, int b)
    {
        pos = a, p = b;
    }
    Node(){}
};

bool cmp(Line a, Line b)
{
    if (a.pos != b.pos)
        return a.pos < b.pos;
    return a.inout < b.inout;
}
int main()
{
    freopen("t.in", "r", stdin);
    freopen("t.txt", "w", stdout);
    int t;
    long long to, te;
    long long tp;
    long long ans1, ans2;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++)
    {
        scanf("%lld%d", &n, &m);
        ans1 = ans2 = 0;
        int tot = 0;
        for (int i = 0; i < m; i++)
        {
            scanf("%lld%lld%lld", &to, &te, &tp);
            long long tmp = (n + n - (te - to) + 1) * (te - to) / 2;
            tmp = ((tmp % MOD) * tp) % MOD;
            ans1 = (ans1 + tmp) % MOD;
            L[tot].inout = 0;
            L[tot].p = tp;
            L[tot++].pos = to;
            L[tot].inout = 1;
            L[tot].p = tp;
            L[tot++].pos = te;
        }
        sort(L, L + tot, cmp);
        stack<Node> st;
        for (int i = 0; i < tot; i++)
        {
            if (L[i].inout == 0)
            {
                st.push(Node(L[i].pos, L[i].p));
                continue;
            }
            while (L[i].p > 0)
            {
                Node k = st.top();
                st.pop();
                if (k.p <= L[i].p)
                {
                    long long tmp = (n + n - (L[i].pos - k.pos) + 1) * (L[i].pos - k.pos) / 2;
                    tmp = ((tmp % MOD) * k.p) % MOD;
                    ans2 = (ans2 + tmp) % MOD;
                    L[i].p -= k.p;
                    continue;
                }
                long long tmp = (n + n - (L[i].pos - k.pos) + 1) * (L[i].pos - k.pos) / 2;
                tmp = ((tmp % MOD) * L[i].p) % MOD;
                ans2 = (ans2 + tmp) % MOD;
                k.p -= L[i].p;
                st.push(k);
                break;
            }
        }
        printf("Case #%d: %lld\n", z, (ans1 - ans2 + MOD) % MOD);
    }
    return 0;
}
