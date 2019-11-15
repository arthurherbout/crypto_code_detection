#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

char st[300],ss[300];
int m,u;
bool check;

void dfs(int k,long long t)
{
    if (k==m)
    {
        int tt=int(sqrt(double(t)));
        for(int i=tt-5;i<=tt+5;++i)
            if (((long long)(i))*i==t)
            {
                ss[m]='\0';
                printf("Case #%d: %s\n",u,ss);
                check=true;
                break;
            }
        return;
    }
    if (st[k]=='0'||st[k]=='?')
    {
        ss[k]='0';
        dfs(k+1,t<<1);
        if (check) return;
    }
    if (st[k]=='1'||st[k]=='?')
    {
        ss[k]='1';
        dfs(k+1,(t<<1)|1);
        if (check) return;
    }
}

int main(void)
{
    freopen("D-small-attempt1.in","r",stdin);
    freopen("D-small-attempt1.out","w",stdout);
    int T,i;
    scanf("%d",&T);
    for(u=1;u<=T;++u)
    {
        scanf("%s",st);
        m=strlen(st);
        check=false;
        dfs(0,0LL);
    }
    return 0;
}
