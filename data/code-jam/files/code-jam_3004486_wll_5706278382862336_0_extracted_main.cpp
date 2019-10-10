#include <bits/stdc++.h>
using namespace std;

int main()
{
    freopen("1.in","r",stdin);
    freopen("1.out","w",stdout);
    int number;
    int n;
    scanf("%d",&n);
    for(number=1; number<=n; number++)
    {
        //start here
        __int64 a,b,t;
        int anss=0;
        bool is=false;
        a=0;
        b=0;
        scanf("%I64d",&a);
        getchar();
        scanf("%I64d",&b);

        here:
        for(t=2; t<=a; t++)
        {
            while(a%t==0&&b%t==0)
            {
                a=a/t;
                b=b/t;
            }
        }

        if(b%2==1||a%2==0)
        {
            printf("Case #%d: impossible\n",number);
            continue;
        }

        int ans=0;
        for(ans=1; ans<=40; ans++)
        {
            a=a*2;
            if(a>=b)
                break;
        }
        if(!is)
        {
            anss=ans;
            is=true;
        }
        a=a-b;
        if(a!=0)
        goto here;


        if(ans<=40)
            printf("Case #%d: %d\n",number,anss);
        else
            printf("Case #%d: impossible\n",number);
    }
    return 0;
}
