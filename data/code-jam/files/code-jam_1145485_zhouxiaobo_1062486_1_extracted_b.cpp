#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

char d[10015][14],L[30];
int s[30][10005],k,g[30],maxn,argmaxn;

void sub(int t,int pa,int sum,int c)
{
    //cout<<endl<<t<<' '<<k<<' '<<pa<<' '<<sum<<endl;
    //cout<<g[t]<<s[t][1]<<s[t][2]<<s[t][3]<<endl;
    if (pa==(1<<k)-1)
    {
        if (maxn<=sum)
        {
            if (maxn<sum||argmaxn>s[t][1])
            {
                maxn=sum;
                argmaxn=s[t][1];
            }
        }
        return;
    }
    int i,j,o,p;
    bool b=false;
    while(!b)
    {
        for(i=1;i<=g[t];++i)
        {
            j=s[t][i];
            for(o=0;o<k;++o)
                if (d[j][o]==L[c])
                {
                    b=true;
                    break;
                }
            if (b)
                break;
        }
        if (b) break;
        ++c;
    }
    for(p=0;p<(1<<k);++p)
        if ((p|pa)==(p^pa))
        {
            g[t+1]=0;
            for(i=1;i<=g[t];++i)
            {
                j=s[t][i];
                b=true;
                for(o=0;o<k;++o)
                {
                    if ((p>>o)&1)
                        b=(d[j][o]==L[c]);
                    else b=(d[j][o]!=L[c]);
                    if (!b)
                        break;
                }
                if (b)
                    s[t+1][++g[t+1]]=j;
            }
            if (g[t+1]>0)
                if (p==0)
                    sub(t+1,pa,sum+1,c+1);
                else
                    sub(t+1,(p|pa),sum,c+1);
        }
}

int main(void)
{
    int T,u,n,m,i,j;
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%d%d",&n,&m);
        for(i=1;i<=n;++i)
            scanf("%s",d[i]);
        printf("Case #%d:",u);
        while(m--)
        {
            scanf("%s",L);
            maxn=-1;
            for(k=1;k<=10;++k)
            {
                g[0]=0;
                for(i=1;i<=n;++i)
                    if (strlen(d[i])==k)
                        s[0][++g[0]]=i;
                if (g[0]>0)
                    sub(0,0,0,0);
            }
            printf(" %s",d[argmaxn]);
        }
        puts("");
    }
    return 0;
}
