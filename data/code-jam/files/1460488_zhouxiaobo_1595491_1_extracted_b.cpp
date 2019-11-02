#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int main(void)
{
    freopen("B-large.in","r",stdin);
    freopen("B-large.out","w",stdout);
    int U,T,n,s,p,i,j,k,t,res;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d%d%d",&n,&s,&p);
        res=0;
        for(i=1;i<=n;++i)
        {
            scanf("%d",&t);
            if (p==1)
            {
                if (t>=1)
                    ++res;
                continue;
            }
            if (t>=(p-2)*3+4)
                ++res;
            else
            if (t>=(p-2)*3+2)
                if (s>0)
                {
                    --s;
                    ++res;
                }
        }
        printf("Case #%d: %d\n",U,res);
    }
    return 0;
}
