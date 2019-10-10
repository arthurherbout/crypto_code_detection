#include<string.h>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
char a[1000005];
int main()
{
int i,j;
int t,ca;
scanf("%d",&t);
for(ca = 1; ca<=t;ca++)
{
    int ans = 0;
    int x,r,c;
    scanf("%d%d%d",&x,&r,&c);
    if(x > r && x>c)ans = 1;
   
    if((r*c)%x)ans = 1;
    
    if(x==3)
    {
        if(r==1 || c==1)ans = 1;
    }
  if(x==4)
  {
      if(r < 3 || c<3)ans = 1;
  }
   printf("Case #%d: ",ca);
   if(ans)
   printf("RICHARD\n");
   else printf("GABRIEL\n");
}

return 0;
}
