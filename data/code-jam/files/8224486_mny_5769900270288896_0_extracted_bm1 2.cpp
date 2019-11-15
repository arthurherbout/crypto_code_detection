#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
int cnt[1 << 18];
int R, C, N;
int un_hap(int i);

int main()
{
        int t1,i,t;
        scanf("%d",&t);
        t1 = t;
        while(t--)
        {
                scanf("%d %d %d",&R,&C,&N);
                int minhap = R * C * N * 100;
                for (i = 1; i < (1 << (R * C)); i++)
                {
                        cnt[i] = cnt[i - (i & -i)] + 1;
                        if (cnt[i] == N)
                               minhap = minhap < un_hap(i)? minhap : un_hap(i);
                }
                printf("Case #%d: %d\n", (t1-t), minhap);
        }
}

int un_hap(int i)
{
        int r,c;
        int unhap = 0;
        for (r = 0; r < R; r++)
        {
                for (c = 0; c < C; c++)
                {
                        if (c > 0 && (i & (1 << (r * C + c))) && (i & (1 << (r * C + c - 1))))
                                unhap++;
                        if (r > 0 && (i & (1 << (r * C + c))) && (i & (1 << (r * C + c - C))))
                                unhap++;
                }
        }
        return unhap;
}

