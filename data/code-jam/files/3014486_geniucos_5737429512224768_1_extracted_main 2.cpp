#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
int cat,pt,T,Ap[1009],a[10009],n,X,i,j,nr;
int main()
{
freopen("input","r",stdin);
freopen("output","w",stdout);
scanf("%d",&T);
while(T)
{
    pt++;
    T--;
    printf("Case #%d: ",pt);
    scanf("%d%d",&n,&X);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&a[i]);
        Ap[a[i]]++;
    }
    sort(a+1,a+n+1);
    nr=0;
    for(i=1;i<=X;i++)
    if(Ap[i])
    {
        for(j=X-i;j>=0;j--)
            if(Ap[j]&&j!=i)
            {
                cat=min(Ap[i],Ap[j]);
                Ap[j]-=cat;
                Ap[i]-=cat;
                nr+=cat;
            }
        if(Ap[i])
        {
            if(i*2<=X) nr+=Ap[i]/2+Ap[i]%2,Ap[i]=0;
            else nr+=Ap[i];
        }
    }
    printf("%d\n",nr);
    for(i=1;i<=X;i++)
        Ap[i]=0;
}
return 0;
}
