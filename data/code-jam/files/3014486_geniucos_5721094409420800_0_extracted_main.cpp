#include<cstdio>
#include<cstring>
#include<vector>
#include<algorithm>
using namespace std;
int p,mini,cat,nr,n,i,j,pt,T,b[1009],c[1009],d[1009];
struct str
{
    int p,v;
}a[1009];
bool cmp(str a,str b)
{
    return a.v<b.v;
}
int mod(int x)
{
    if(x<0) return -x;
    return x;
}
void U()
{
    int i,j,cost=0;
    for(i=1;i<=n;i++)
        d[i]=b[i];
    for(i=1;i<=n;i++)
    {
        for(j=i;j<=n;j++)
            if(d[j]==c[i]) break;
        for(j=j;j>i;j--)
            swap(d[j],d[j-1]),cost++;
    }
    if(cost<mini)
        mini=cost;
}
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
    scanf("%d",&n);
    for(i=1;i<=n;i++)
    {
        scanf("%d",&a[i].v);
        a[i].p=i;
    }
    sort(a+1,a+n+1,cmp);
    for(i=1;i<=n;i++)
        b[a[i].p]=i;
    mini=100000000;
    for(j=0;j<(1<<(n-1));j++)
    {
        nr=0;
        for(p=0;p<n-1;p++)
            if(j&(1<<p)) c[++nr]=p+1;
        c[++nr]=n;
        for(p=n-2;p>=0;p--)
            if((j&(1<<p))==0) c[++nr]=p+1;
        U();
    }
    printf("%d\n",mini);
}
return 0;
}
