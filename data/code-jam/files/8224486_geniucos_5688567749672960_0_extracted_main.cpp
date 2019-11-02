#include<cstdio>
#include<vector>
#include<cstring>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<map>
#include<set>
#include<queue>
#include<algorithm>

using namespace std;

int N, dp[1000009];
queue < int > cc;

int rev (int x)
{
    int sol = 0;
    while (x)
        sol = sol * 10 + x % 10, x /= 10;
    return sol;
}

int main()
{
//freopen ("input", "r", stdin);
//freopen ("output", "w", stdout);

cc.push (1);
dp[1] = 1;
while (!cc.empty ())
{
    int nod = cc.front ();
    cc.pop ();
    if (dp[nod + 1] == 0 && nod + 1 <= 1000000)
    {
        dp[nod + 1] = dp[nod] + 1;
        cc.push (nod + 1);
    }
    if (dp[rev (nod)] == 0 && rev (nod) <= 1000000)
    {
        dp[rev(nod)] = dp[nod] + 1;
        cc.push (rev (nod));
    }
}

int test = 0, T;
scanf ("%d", &T);
while (test < T)
{
    printf ("Case #%d: ", ++test);
    scanf ("%d", &N);
    printf ("%d\n", dp[N]);
}

return 0;
}
