#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll po[65];
char ans[55][55];
int main() {
    int t,ca;
    scanf("%d",&t);
    po[0] = 1;
    for(ca=1;ca<=55;ca++)
        {
        po[ca] = 2* po[ca-1];
    }
    for(ca=1;ca<=t;ca++)
        {
     int b;
        ll m;
       scanf("%d%lld",&b,&m);
        int n = b;
      if(m > po[b-2])
          {
          printf("Case #%d: IMPOSSIBLE\n",ca);
      }
        else {
            printf("Case #%d: POSSIBLE\n",ca);
            int i,j;
            for(i=0;i<n;i++)
                {
                for(j=0;j<n;j++)
                    {
                    ans[i][j] = '0';
                }
                ans[i][j] = 0;
                 for(j=i+1;j<n-1;j++)
                    {
                    ans[i][j] = '1';
                }
                
            }
            int in = n-3;
            for(i=n-2;i>0;i--)
                {
                if(m>=po[in])
                    {
                    m-=po[in];
                    ans[i][n-1] = '1';
                }
                in--;
            }
            if(m)ans[0][n-1] = '1';
            for(i=0;i<n;i++)
                printf("%s\n",ans[i]);
        }
      
    }
    return 0;
}

