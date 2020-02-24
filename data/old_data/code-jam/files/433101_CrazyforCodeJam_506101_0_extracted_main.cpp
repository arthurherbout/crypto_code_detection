#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <iterator>
using namespace std;

int c,n,t[15];
int a,b;

int gcd(int a,int b)
{
    if (a == 0)
    return b;
    return gcd(b%a,a);
}
int main()
{
    freopen("B-small-attempt3.in","r",stdin);
    freopen("B-small-attempt3.out","w",stdout);

    scanf("%d",&c);
    int i,j;
    //printf("gcd=%d\n",gcd(82,12));
    for (i=0;i<c;i++)
    {
        scanf("%d",&n);
        for (j=0;j<n;j++)
        {
            scanf("%d",t+j);
        }
        if (n==2)
        {
                 a = t[0]<t[1]?t[0]:t[1];
                 b = t[0]<t[1]?t[1]:t[0];
                 int d = b - a;
                 int ans = (a%d)>0?a%d-d:0;
                 printf("Case #%d: %d\n",i+1,-ans);
        }
        else if (n==3)
        {
            sort(t,t+3);
            t[3]=t[2]-t[1];
            t[4]=t[1]-t[0];
            a=t[3]<t[4]?t[3]:t[4];
            b=t[3]<t[4]?t[4]:t[3];
            int d=a;
			if (a==0)
			{
                 int d = b ;
                 int ans = (t[0]%d)>0?t[0]%d-d:0;
                 printf("Case #%d: %d\n",i+1,-ans);
			}
			else
            if (b%a==0)
            {
                       int ans = t[0]%d>0?t[0]%d-d:t[0]%d;
                       printf("Case #%d: %d\n",i+1,-ans);
            }
            else
            {
                int g;
                g = gcd(a,b);
                int ans = t[0]%g>0?t[0]%g-d:0;
                printf("Case #%d: %d\n",i+1,-ans);
            }
        }
    }

return 0;
}
