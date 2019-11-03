#include <stdio.h>



int main()
{
    freopen("B-small-attempt1.in", "r", stdin);
    freopen("soutb1.out", "w", stdout);
    int test, tag, tmp;
    int n, m, a, t = 1;
    int x1, y1, x2, y2, x3, y3;
    scanf("%d", &test);
    while(test--)
    {
       
       
        tag = 0;
        scanf("%d%d%d", &n, &m, &a);
        printf("Case #%d: ", t++);
        if(a > n * m)
        {
            printf("IMPOSSIBLE\n");
            continue;
        }
        for(x1 = 0;x1 <= n;x1++)
        for(y1 = 0;y1 <= m;y1++)
        for(x2 = 0;x2 <= n;x2++)
        for(y2 = 0;y2 <= m;y2++)
        for(x3 = 0;x3 <= n;x3++)
        for(y3 = 0;y3 <= m;y3++)
        {
            tmp = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
            if(tmp == a || tmp == -a)
            {
                tag = 1;
                goto lib;
            }
        }
        lib :
        if(tag == 1) printf("%d %d %d %d %d %d\n", x1, y1, x2, y2, x3, y3);
        else printf("IMPOSSIBLE\n");
    }
    return 0;   
}

