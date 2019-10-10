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
    int tst,cas=0,s,frnd,peo,i,j;
    //freopen("A-small-attempt0.in.txt","r",stdin);
    //freopen("A-small-attempt0.in_out.txt","w",stdout);
    char str[1005];
    scanf("%d", &tst);
    while(tst--)
    {
        scanf("%d %s", &s, str);
        frnd=0;
        peo=str[0]-'0';
        for(i=1;i<=s;i++)
        {
            if(str[i]!='0')
            {
                if(peo>=i)
                 {
                 peo+=str[i]-'0';
                 }
             else
                {
                 frnd+=(i-peo);
                 peo+=(i-peo);
                 peo+=str[i]-'0';
                }
            }
            // cout<<"i "<<i<<" peo "<<peo<<" frnd "<<frnd<<endl;
        }
       // cout<<"peo "<<peo<<" frnd "<<frnd<<endl;
        printf("Case #%d: %d\n",++cas,frnd);
    }
    return 0;
}

