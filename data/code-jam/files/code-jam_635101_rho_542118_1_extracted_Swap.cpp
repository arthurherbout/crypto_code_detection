#include<stdio.h>

bool canReachBarn(long pos, long speed, long goal, long time)
{
    //printf("%ld %ld %ld\n", speed*time,goal, pos);
    return (speed*time>=goal-pos);
}

int main(void)
{
    FILE* fin = fopen("swap.in", "r");
    FILE* fout = fopen("swap.out", "w");  
    int n;
    fscanf(fin, "%d", &n);
    for(int i=1; i<=n; i++)
    {
        long total, inBarn, Barn, time;
        fscanf(fin, "%ld %ld %ld %ld", &total, &inBarn, &Barn, &time);
        long chicks[total][2];
        for(int j=0; j<total; j++)
        {
            fscanf(fin, "%ld", &chicks[j][0]);
        }
        for(int j=0; j<total; j++)
        {
            fscanf(fin, "%ld", &chicks[j][1]);
        }
        /*for(int j=0; j<total; j++)
        {
            printf("%ld %ld\n", chicks[j][0],chicks[j][1]);
        }*/
        int c=0;
        int d=0;
        int s=0;
        for(int j=total-1; j>=0 && c<inBarn; j--)
        {
            if(canReachBarn(chicks[j][0], chicks[j][1], Barn, time))
            {
                c++;   
                s+=d;
            }
            else
            {
                d++;
            }
        }
        if(c>=inBarn)
            fprintf(fout, "Case #%d: %d\n",  i, s);
        else
            fprintf(fout, "Case #%d: IMPOSSIBLE\n",  i);
   }
   //getchar();
}

