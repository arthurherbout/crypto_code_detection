#include<stdio.h>

int main()
{
    void testcase(int num);
    int t, i;
    scanf("%d",&t);
    for(i=1; i<=t; i++)
    {
    testcase(i);
    }
    return 0;
}

void testcase(int num)
{
    int a, b, k,p,q,r, count = 0;
    scanf("%d%d%d",&a,&b,&k);
    for(p=0;p<a; p++)
    {
    for(q=0;q<b;q++)
    {
    if((p & q)<k)
    count ++;
    }
    }
    printf("Case #%d: %d\n",num,count);
    }
