#include <iostream>
#include <algorithm>
using namespace std;
char s[42];
int num[42],len;
long long ans;
void dfs(int p,long long t)
{
    int i;
    long long k;
    if (p<len)
    {
        k=0;
        for (i=p;i<len;i++)
        {
            k=k*10+num[i];
            dfs(i+1,t+k);
            dfs(i+1,t-k);
        }
    }
    else
        if (t%2==0||t%3==0||t%5==0||t%7==0)
            ans++;
}
int main()
{
    freopen("d:\\B-small-attempt0.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,t,n;
    long long k;
    scanf("%d",&n);
    for (t=1;t<=n;t++)
    {
        scanf("%s",s);
        for (len=0;s[len];len++)
            num[len]=s[len]-'0';
        ans=0;
        k=0;
        for (i=0;i<len;i++)
        {
            k=k*10+num[i];
            dfs(i+1,k);
        }
        printf("Case #%d: %lld\n",t,ans);
    }
    return 0;
}