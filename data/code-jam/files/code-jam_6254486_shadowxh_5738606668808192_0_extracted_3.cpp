#include<stdio.h>
#include<string.h>
#include<string>
#include<algorithm>
#include<iostream>
using namespace std;
typedef long long ll;
int n,m;
ll a[20];
ll b[20];
int cnt;
bool is_prime(ll num)
{
    ll i,j;
    for(i=2;i<num;i++)
        if(num%i==0) return 0;
    return 1;
}
void dfs(int cur,int num)
{
    ll i,j;
    a[cur]=num;
    if(cur==n-1)
    {
        for(i=2;i<=10;i++)
        {
            ll tmp=0,bx=1;
            for(j=n;j>=1;j--)
            {
                tmp+=a[j]*bx;
                bx=bx*i;
            }
            b[i]=tmp;
            if(is_prime(tmp)) break;
        }
        if(i!=11) return;
        cnt++;
        if(cnt>m) return;
        for(i=1;i<=n;i++) printf("%d",a[i]);
        for(i=2;i<=10;i++)
        {
            for(j=2;j<b[i];j++)
                if(b[i]%j==0) {printf(" %d",j);break;}
        }
        printf("\n");
        return;

    }
    if(cur>6)
    {
        for(i=0;i<=1;i++)
        {
            dfs(cur+1,i);
        }
    }
    else
    {
        for(i=1;i>=0;i--)
        {
            dfs(cur+1,i);
        }
    }
    return;
}
int main()
{
    //freopen("C-small-attempt.out","r",stdin);
    //freopen("C-small-attempt0.out","w",stdout);
    int i,j;
    int t;
    a[1]=1;
    scanf("%d",&t);
    cnt=0;
    scanf("%d%d",&n,&m);
    a[n]=1;
    printf("Case #1:\n");
    for(i=0;i<=1;i++)
        dfs(2,i);
    return 0;
}

