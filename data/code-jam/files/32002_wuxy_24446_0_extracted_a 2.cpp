#include <iostream>
#include <algorithm>
using namespace std;
int a[110],f[110][2],bf[90];
char s[90][90];
int main()
{
    freopen("C-small-attempt3.in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    int i,j,k,t,g,tt,m,n,ans;
    scanf("%d",&tt);
    for (t=1;t<=tt;t++)
    {
        scanf("%d%d",&m,&n);
        memset(a,0,sizeof(a));
        memset(f,0,sizeof(f));
        for (i=0;i<m;i++)
        {
            scanf("%s",s[i]);
            for (j=0;j<n;j++)
                if (s[i][j]=='.')
                    a[j]++;
        }
        ans=0;
        f[0][1]=a[0];
        f[1][1]=a[1];
        f[1][0]=a[0];
        bf[0]=1;
        if (a[0]>=a[1])
            bf[1]=0;
        else
            bf[1]=1;
        for (i=2;i<n;i++)
        {
            f[i][0]=f[i-1][bf[i-1]];
            f[i][1]=f[i-2][bf[i-2]]+a[i];
            if (bf[i-2]==1)
            {

                for (j=0;j<m;j++)
                    if (s[j][i]=='x')
                    {
                        for (k=j+1;k<m;k++)
                            if (s[k][i]=='x')
                                break;
                        if (k<m)
                        {
                            for (g=j+1;g<=k;g++)
                                if (s[g][i-1]=='x')
                                    break;
                            if (g>k)
                            {
                                for (g=j;g<=k;g++)
                                    if (s[g][i-2]=='.')
                                        break;
                                if (g>k)
                                    f[i][1]++;
                            }
                        }
                        k=j-1;
                    }

            }
            else
            {
                for (j=0;j<m;j++)
                    if (s[j][i]=='x')
                    {
                        for (k=j+1;k<m;k++)
                            if (s[k][i]=='x')
                                break;
                        if (k<m)
                        {
                            for (g=j+1;g<=k;g++)
                                if (s[g][i-1]=='x')
                                    break;
                            if (g>k)
                                f[i][1]++;
                        }
                        k=j-1;
                    }
            }
            if (f[i][1]>f[i][0])
            {
                bf[i]=1;
                ans=f[i][1];
            }
            else
            {
                bf[i]=0;
                ans=f[i][0];
            }
        }
        printf("Case #%d: %d\n",t,ans); 
    }
    return 0;
}