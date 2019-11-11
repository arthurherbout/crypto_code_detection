/*
  "I have nothing to loose
  so,nothing to fear"
  motto:think out of the box
*/
#include<cstdio>
#include<cstring>
#include<cctype>
#include<cmath>
#include<cstdlib>
#include<limits.h>
#include<iostream>
#include<string>
#include<vector>
#include<stack>
#include<queue>
#include<map>
#include<set>
#include<deque>
#include<algorithm>
#define PI (2* acos(0))
#define pb push_back
#define ll long long
using namespace std;
//int X[]= {0,0,1,-1};
//int Y[]= {-1,1,0,0};
//int X[]= {0,0,1,1,1,-1,-1,-1};
//int Y[]= {-1,1,0,1,-1,0,1,-1};
int main()
{
    int n,m,x,r,c,tst,cas=0;
  //  freopen("D-small-attempt0.in.txt","r",stdin);
   // freopen("D-small-attempt0.in_out.txt","w",stdout);
    scanf("%d", &tst);
    while(tst--)
    {
        scanf("%d %d %d", &x, &r, &c);
        n=min(r,c);
        m=max(r,c);
        if(x==1)
        {
            printf("Case #%d: GABRIEL\n",++cas);
        }
        else if(x==2)
        {
            if((n==1 && m==3) || (n==1 && m==1) || (n==3 && m==3))
                printf("Case #%d: RICHARD\n",++cas);
            else
                printf("Case #%d: GABRIEL\n",++cas);

        }
        else if(x==3)
        {
            if((n==3 && m==4) || (n==2 && m==3) || (n==3 && m==3))
                 printf("Case #%d: GABRIEL\n",++cas);
            else
                 printf("Case #%d: RICHARD\n",++cas);
        }
        else
        {
            if((n==3 && m==4) || (n==4 && m==4))
               printf("Case #%d: GABRIEL\n",++cas);
            else
                 printf("Case #%d: RICHARD\n",++cas);
        }

    }

    return 0;
}

