#include<cstdio>
#include<algorithm>
using namespace std;
int i,j,nr,A,B,C,pt,T;
int main()
{
//freopen("input","r",stdin);
//freopen("output","w",stdout);
scanf("%d",&T);
while(T)
{
    pt++;
    T--;
    printf("Case #%d: ",pt);
    scanf("%d%d%d",&A,&B,&C);
    nr=0;
    for(i=0;i<A;i++)
        for(j=0;j<B;j++)
            if((i&j)<C) nr++;
    printf("%d\n",nr);
}
return 0;
}
