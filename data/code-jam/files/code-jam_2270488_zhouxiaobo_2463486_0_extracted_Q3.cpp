#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>

using namespace std;

long long s[100];

bool check(int x)
{
    int z = x, y = 0;
    while(x > 0)
    {
        y = y * 10 + x % 10;
        x /= 10;
    }
    return y == z;
}

int main(void)
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
    FILE *fp = fopen("list.txt","r");
    int n = 0, i, U, T;
    long long a, b;
    while(fscanf(fp, "%d%I64d", &i, s + n) != EOF)
    {
        long long x = sqrt(double(s[n]));
        if (x * x != s[n])
        {
           cout << "error" << endl;
        }
        if (check((int)(x)))
            ++n;
    }
    fclose(fp);
    scanf("%d", &T);
    for(U = 1; U <= T; ++U)
    {
        scanf("%I64d%I64d", &a, &b);
        int res = 0;
        for(i = 0; i < n; ++i)
            if (a <= s[i] && s[i] <= b)
                res++; 
        printf("Case #%d: %d\n", U, res);
    }
    return 0;
}
