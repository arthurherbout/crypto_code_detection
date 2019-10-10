#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

using namespace std;

map < int, int > MP;

void solve(int test)
{
    MP.clear();
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i ++)
    {
        int a, b;
        scanf("%d%d", &a, &b);
        MP[a] = b;
    }

    int res = 0;
    while (true)
    {
        bool good = false;
        for (map < int, int > :: iterator it = MP.begin(); it != MP.end(); it ++)
        {
            int A = it->first, B = it->second;
            if (B >= 2)
            {
                MP[A - 1] ++;
                MP[A + 1] ++;
                MP[A] -= 2;
                res ++;
                good = true;
                break;
            }
        }
        if (!good) break;
    }
    printf("Case #%d: %d\n", test, res);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);

    int test;
    scanf("%d", &test);
    for (int i = 1; i <= test; i ++)
        solve(i);
    return 0;
}