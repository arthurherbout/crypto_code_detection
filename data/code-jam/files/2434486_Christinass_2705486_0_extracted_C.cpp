#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;
#define maxn 100005

int n;
struct node
{
       long long k,a;
}b[maxn];
long long p[30];
bool cmp(node x,node y)
{
     return (x.k<y.k);
}

int main()
{
    scanf("%d",&n);
    p[0]=1;
    for (int i=1;i<=29;i++)
        p[i]=p[i-1]*4;
    for (int i=1;i<=n;i++)
        scanf("%I64d%I64d",&b[i].k,&b[i].a);
    sort(b+1,b+n+1,cmp);
    for (int i=1;i<n;i++)
    {
        long long int tmp=b[i+1].k-b[i].k;
        double t=log(1.0*b[i].a)-log(4.0)*tmp;
        long long int x;
        if (tmp<=29&&b[i].a%p[tmp]==0) x=b[i].a/p[tmp];
        else x=(long long int)(exp(t))+1;
        b[i+1].a=max(x,b[i+1].a);
    }
    if (b[n].a==1) printf("%I64d\n",b[n].k+1);
    else
    {
        int i;
        for (i=0;i<29;i++)
            if (p[i]<=b[n].a&&b[n].a<p[i+1]) break;
        printf("%I64d\n",b[n].k+i+(!(p[i]==b[n].a)));
    }
    return 0;
}
