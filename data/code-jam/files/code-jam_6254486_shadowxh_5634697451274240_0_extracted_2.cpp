#include<stdio.h>
#include<string.h>
int n;
int a[11];
bool vis[11];
char s[15];
void cal(char str[],int num)
{
    int i,j;
    for(i=0;i<num/2;i++){str[i]^=str[num-1-i];str[num-1-i]^=str[i];str[i]^=str[num-1-i];}
    for(i=0;i<num;i++) str[i]='+'-str[i]+'-';
}
bool judge(char t[])
{
    int i;
    for(i=0;i<n;i++)
        if(t[i]!='+') break;
    if(i==n) return 1;
    return 0;
}
int ans;
void dfs(int cur,int num)
{
    int i,j;
    vis[num]=1;
    a[cur]=num;
    if(cur==n)
    {
        //for(i=1;i<=n;i++) printf("%d ",a[i]);printf("\n");
        char t[15];
        for(i=0;i<=n;i++) t[i]=s[i];
        for(i=1;i<=n;i++)
        {
            cal(t,a[i]);
            //for(j=0;j<n;j++) printf("%c ",t[j]);printf("\n");
            if(judge(t) && i<ans) {ans=i;break;}
        }
        vis[num]=0;
        return;
    }
    for(i=1;i<=n;i++)
    {
        if(vis[i]) continue;
        dfs(cur+1,i);

    }
    vis[num]=0;
    return;
}
int main()
{
    //freopen("B-small-attempt0.in","r",stdin);
    //freopen("B-small-attempt0.out","w",stdout);
    int i,j;
    int t,cases=0;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%s",s);
        n=strlen(s);
        for(i=0;i<n;i++)
        {
            if(s[i]!='+') break;
        }
        if(i==n)
        {
            printf("Case #%d: 0\n",++cases);
            continue;
        }
        ans=n;
        memset(vis,0,sizeof(vis));
        for(i=1;i<=n;i++)
        {
            dfs(1,i);
        }
        printf("Case #%d: %d\n",++cases,ans);
    }


    return 0;
}
