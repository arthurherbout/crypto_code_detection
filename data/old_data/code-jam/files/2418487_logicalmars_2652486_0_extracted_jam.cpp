#include <cstring>
#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
using namespace std;

int r, n, m, k;
int pt[20];
int num[20];

int get_rand()
{
    return rand() % (m - 1) + 2;
}

int get_product(int mask)
{
    int res = 1;
    for (int i = 0;i<n;i++)
    {
        if ((1<<i) & mask)
        {
            res *= num[i];
        }
    }

    return res;
}

void guess()
{
    int er_top = 1;
    for (int i = 0;i<n;i++)
        er_top *= 2;

    while (1)
    {        
        for (int i = 0;i<n;i++)
            num[i] = get_rand();

        int all_products[200];
        memset(all_products, 0, sizeof(all_products));

        for (int mask = 0; mask < er_top; mask++)
        {
            int temp = get_product(mask);
            all_products[temp] = 1;
        }

        int flag = 0;
        for (int i = 0;i<k;i++)
        {
            if (all_products[pt[i]] == 0)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            for (int i = 0;i<n;i++)
                printf("%d", num[i]);
            printf("\n");
            return ;
        }
    }
}

void work()
{
    scanf("%d%d%d%d", &r, &n, &m, &k);

    for (int i = 0;i<r;i++)
    {
        for (int j = 0;j<k;j++) scanf("%d", &pt[j]);
        guess();
    }
}
 
int main()
{
    freopen("C-small-1-attempt0.in", "r", stdin);
    // freopen("A-large-practice.in", "r", stdin);    
    // freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
 
    int cs, t;
    scanf("%d", &t);
    for (int cs = 1; cs <= t; cs++)
    {
        printf("Case #%d:\n", cs);
        work();
    }
 
    return 0;
}