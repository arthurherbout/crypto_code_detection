#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

int f[]={1,10,100,1000,10000,100000,1000000,10000000};

int main(void)
{
    freopen("C-small-attempt0.in","r",stdin);
    freopen("C-small-attempt0.out","w",stdout);
    int T,A,B,U,n,m,i,j,k,d,res;
    scanf("%d",&T);
    for(U=1;U<=T;++U)
    {
        scanf("%d%d",&A,&B);
        res=0;
        for(n=A;n<B;++n)
        {
            d=0;
            while(n>=f[d])
                ++d;
            for(i=1;i<d;++i)
            {
                m=n/f[i]+n%f[i]*f[d-i];
                if (n<m&&m<=B)
                {
                    //cout<<n<<' '<<m<<' '<<endl;
                    ++res;
                }
            }
        }
        printf("Case #%d: %d\n",U,res);
    }
    //system("PAUSE");
    return 0;
}
