#include <iostream>
#include <algorithm>

using namespace std;

int main()
{
    freopen("A-small-attempt2.in", "r", stdin);
    freopen("as.out", "w", stdout);
    int a[100], b[100], c[100];
    int test, i, t = 1, n, sum, ans;
    scanf("%d", &test);
    while(test--)
    {
        scanf("%d", &n);
        for(i = 0;i < n;i++)
            scanf("%d", &a[i]);
        for(i = 0;i < n;i++)
            scanf("%d", &b[i]);
        for(i = 0;i < n;i++)
            c[i] = i;
        ans = 0x7fffffff;
        sum = 0;
        for(i = 0;i < n;i++)
        {
            sum += a[i] * b[c[i]];
        }
        if(sum < ans) ans = sum;
        while(next_permutation(c, c + n))
        {
            sum = 0;
            for(i = 0;i < n;i++)
            {
                sum += a[i] * b[c[i]];
            }
            if(sum < ans) ans = sum;
        } 
        printf("Case #%d: %d\n", t++, ans); 
    }
    return 0;
} 
