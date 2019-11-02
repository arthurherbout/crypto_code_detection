#include<stdio.h>
int main()
{
    int t,n,m;
    int a[105][105];
    int i,j,k;
    int ca;
    scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
    {
                        int valid=1;
                        scanf("%d%d",&n,&m);
                        for(i=0;i<n;i++)
                        for(j=0;j<m;j++)
                        scanf("%d",&a[i][j]);
                        
                        for(i=0;i<n;i++)
                        {
                                        int hi=0;
                                        for(j=0;j<m;j++)if(a[i][j]>hi)hi=a[i][j];
                                        for(j=0;j<m;j++)
                                        {
                                                        if(a[i][j]<hi){
                                                                       for(k=0;k<n;k++)if(a[k][j]>a[i][j])valid=0;
                                                                       }
                                                        }
                                        }
                                         for(i=0;i<m;i++)
                        {
                                        int hi=0;
                                        for(j=0;j<n;j++)if(a[j][i]>hi)hi=a[j][i];
                                        for(j=0;j<n;j++)
                                        {
                                                        if(a[j][i]<hi){
                                                                       for(k=0;k<m;k++)if(a[j][k]>a[j][i])valid=0;
                                                                       }
                                                        }
                                        }
                                        if(valid)printf("Case #%d: YES\n",ca);
                                        else printf("Case #%d: NO\n",ca);
                        }
}
