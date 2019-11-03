#include <iostream>
#include <algorithm>
using namespace std;
int a[8],flag[8],k,ans,len;
char s[1010];
void change()
{
    int i,tot,g;
    char p,q;
    p='Z';
    tot=0;
    for (i=0;i<len;i+=k)
    {
        for (g=1;g<=k;g++)
        {
            q=s[a[g]+i-1];
            if (p!=q)
            {
                tot++;
                p=q;
            }
        }
    }
    if (tot<ans)
        ans=tot;
}
void dfs(int dep)
{
    int i;
    if (dep>k)
    {
        change();
        return;
    }
    for (i=1;i<=k;i++)
        if (flag[i])
        {
            flag[i]=0;
            a[dep]=i;
            dfs(dep+1);
            flag[i]=1;
        }
}
int main()
{
    freopen("d:\\D-small-attempt0.in.txt","r",stdin);
    freopen("d:\\out.txt","w",stdout);
    int i,t,tt,m,n;
    scanf("%d",&tt);
    for (t=1;t<=tt;t++)
    {
        scanf("%d",&k);
        scanf("%s\n",s);
        memset(flag,1,sizeof(flag));
        ans=1000;
        for (i=0;s[i];i++);
        len=i;
        dfs(1);
        printf("Case #%d: %d\n",t,ans); 
    }
    return 0;
}