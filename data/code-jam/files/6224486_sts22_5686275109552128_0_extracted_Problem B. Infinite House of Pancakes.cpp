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
    int tst,cas=0,d,p[1005],ans,mx,pos,i,j,cnt,res;
     //freopen("B-small-attempt0.in.txt","r",stdin);
    // freopen("B-small-attempt0.in_out.txt","w",stdout);
    scanf("%d", &tst);
    while(tst--)
    {
        scanf("%d", &d);
        mx=0;
        for(i=0;i<d;i++)
        {
             scanf("%d", &p[i]);
             if(p[i]>mx)
             {
                   mx=p[i];
                   pos=i;
             }

        }
        if(mx==1)
        {
            printf("Case #%d: %d\n",++cas,mx);
        }
        else
        {
           // cnt=1;
           // int tmp=pos;

            res=1<<28;
            int num=mx;
            for(i=1;i<=num;i++)
            {
                for(j=0;j<d;j++)
                {
                    if(p[j]>mx)
                    {
                        mx=p[j];
                        pos=j;
                    }
                }
                if(mx & 1)
                 {
                ans=mx>>1;
                ans+=1;
                 }
                else
                 {
                ans=mx>>1;
                 }
              p[pos]=ans;
              ans+=i;
              if(res>ans)
              {
                  res=ans;
              }

            }

             printf("Case #%d: %d\n",++cas,res);
        }

    }
    return 0;
}

