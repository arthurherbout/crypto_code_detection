#include<bits/stdc++.h>
using namespace std;
double p[105];
int main()
{
    freopen("C-small-1-attempt1.in","r",stdin);
    freopen("c-s1.out","w",stdout);
    int t;
    scanf("%d",&t);
    for(int cases=1;cases<=t;cases++)
    {
        int n,k;
        double u;
        scanf("%d%d%lf",&n,&k,&u);
        //printf("%d %d\n%lf\n",n,k,u);
        for(int i=0;i<n;i++)
        {
            scanf("%lf",&p[i]);
        }
        for(int i=0;i<n;i++)
        {
            //printf("%lf ",p[i]);
        }
        //printf("\n");
        sort(p,p+n);
        p[n]=1.1;
        int f=0;
        while(u>0.0)
        {
            while(p[f+1]==p[f])f++;
            double dif=p[f+1]-p[f];
            double add;
            if(dif*(f+1)<u)add=dif;
            else add=u/(f+1);
            for(int i=0;i<=f;i++)
            {
                p[i]=p[i]+add;
                u=u-add;
            }
        }
        double ans=1.0;
        for(int i=0;i<n;i++)
        {
            ans=ans*p[i];
        }
        printf("Case #%d: %lf\n",cases,ans);
    }
    return 0;
}
