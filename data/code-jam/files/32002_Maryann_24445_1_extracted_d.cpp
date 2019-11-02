#include<cstdio>
#include<algorithm>
using namespace std;
char s[1005],w[1005];
int n,m,k,ans,a[5];
int compress()
{
    int ret=1;
    for(int i=1; i<m; i++)
    if(w[i]!=w[i-1])
    ret++;
    return ret;
}
int main()
{
    int _,num;
    scanf("%d",&_);
    for(int t=1; t<=_; t++)
    {
        scanf("%d%s",&k,s);
        n=(m=strlen(s))/k;
        for(int i=0; i<k; i++)
        a[i]=i;
        num=1;
        for(int i=2; i<=k; i++)
        num*=i;
        ans=m;
        while(num--)
        {
            for(int i=0,t=0; i<n; i++)
            for(int j=0; j<k; j++,t++)
            w[i*k+a[j]]=s[t];
            ans=min(ans,compress());
            next_permutation(a,a+k);
        }
        printf("Case #%d: %d\n",t,ans);
    }
    return 0;
}