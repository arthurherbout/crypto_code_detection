#include<iostream>
#include<vector>
#include<stdio.h>
using namespace std;
int main()
{
    long long i,j,k,l,m,n,r,t;
    scanf("%lld",&m);
    for(i=1;i<=m;i++)
    {
        scanf("%lld",&r);
        scanf("%lld",&t);
        l=r*2;
        l=l+1;
        k=0;
        while(t>0)
        {
            if((t-l)>=0)
            {
                k++;
            }
            t=t-l;
            l=l+4;
        }
        printf("Case #%lld: %lld\n",i,k);
    }
}
