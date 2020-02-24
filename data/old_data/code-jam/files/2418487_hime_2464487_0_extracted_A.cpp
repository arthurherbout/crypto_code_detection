#include<cstdio>

short T,ca;
long long r,t;
int i,j,k,ans;

inline long long sq(int a)
{
    return a*a;
}

int main()
{
    scanf("%hd",&T);
    for(ca=1;ca<=T;++ca)
    {
        scanf("%lld %lld",&r,&t);
        for(ans=0,++r;sq(r)-sq(r-1)<=t;++ans,r+=2)
            t-=sq(r)-sq(r-1);
        printf("Case #%hd: %d\n",ca,ans);
    }
    return 0;
}
