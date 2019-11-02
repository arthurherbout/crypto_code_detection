#include <iostream>
#include <cstdio>
#include <string.h>
#include <cstring>
using namespace std;

bool judge(char ch)
{
    if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u')
        return false;
    return true;
}

char str[1000010];
int L;
int main()
{
    freopen("t.in", "r", stdin);
    freopen("ans.txt", "w", stdout);
    int t;
    scanf("%d", &t);
    for (int z = 1; z <= t; z++)
    {
        getchar();
        scanf("%s%d", str, &L);
        int len = strlen(str);
        int i = 0;
        int j = 0;
        long long ans = 0;
        int cntlen = 0;
        while(i < len && j < len)
        {
            while(j < len)
            {
                if (judge(str[j]))
                {
                    cntlen++;
                    if (cntlen == L)
                        break;
                }
                else
                    cntlen = 0;
                j++;
            }
            while (cntlen == L)
            {
                ans += ((long long)(len - j));
                //printf("i = %d, j = %d, ans = %lld\n", i, j, ans);
                i++;
                if (i > j - L + 1)
                    cntlen--;
            }
            j++;
        }
        printf("Case #%d: %lld\n", z, ans);
    }
    return 0;
}
