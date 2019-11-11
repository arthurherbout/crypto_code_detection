#include <string.h>
#include <stdio.h>
int p,q;
int min;
const int maxnum = 0x7fffffff/4;
short used[10],list[10];
int people[10];
short house[110];
void go(int x)
{
        if (x>q)
        {               
                int sum=0; 
                int i,j;
                for (i=1;i<=p;i++) house[i]=1;
                for (i=1;i<=q;i++)
                {
                        j=people[list[i]];
                        int k;
                        for (k=j-1;k>=1 && house[k];k--) sum++;
                        for (k=j+1;k<=p && house[k];k++) sum++;
                        house[j]=0;
                }
                if (sum < min) min = sum;
        }
        int i;
        for (i=1;i<=q;i++) 
        {
                if (used[i]) continue;
                list[x] = i;
                used[i]=1;
                go(x+1);
                used[i]=0;
        }
}
                
int main()
{
        freopen("c.in","r",stdin);
        freopen("c.out","w",stdout);
        int css;
        scanf("%d",&css);
        int cs;
        for (cs=1;cs<=css;cs++)
        {                
                scanf("%d%d",&p,&q);
                memset(used,0,sizeof(used));
                int i;
                for (i=1;i<=q;i++) scanf("%d",&people[i]);
                min=maxnum;
                go(1);
                printf("Case #%d: %d\n",cs,min);
        }
        return 0;
}
