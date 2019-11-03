#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

char sc[40][4],sd[40][3];
char res[120],st[120];

int main(void)
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    int u,T,c,d,n,m,i,j,k;
    bool check;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d",&c);
        for(i=1;i<=c;++i)
            scanf("%s",sc[i]);
        scanf("%d",&d);
        for(i=1;i<=d;++i)
            scanf("%s",sd[i]);
        scanf("%d",&n);
        scanf("%s",st);
        m=0;
        for(i=0;i<n;++i)
        {
            if (m==0)
            {
                res[++m]=st[i];
                continue;
            }
            check=true;
            for(j=1;j<=c;++j)
                if ((res[m]==sc[j][0]&&st[i]==sc[j][1])
                  ||(res[m]==sc[j][1]&&st[i]==sc[j][0]))
                {
                    res[m]=sc[j][2];
                    check=false;
                    break;
                }
            if (!check)
                continue;
            for(j=1;j<=d;++j)
            {
                for(k=1;k<=m;++k)
                    if ((res[k]==sd[j][0]&&st[i]==sd[j][1])
                      ||(res[k]==sd[j][1]&&st[i]==sd[j][0]))
                    {
                        m=0;
                        check=false;
                        break;
                    }
                if (!check)
                    break;
            }
            if (!check)
                continue;
            res[++m]=st[i];
        }
        printf("Case #%d: [",u);
        for(i=1;i<=m;++i)
        {
            printf("%c",res[i]);
            if (i<m)
                printf(", ");
        }
        puts("]");
    }
    return 0;
}
