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
   int req;
   int n;
   scanf("%d",&n);
   scanf("%s",&a);
   int total = 0;
   for(i=0;i<=n;i++)
   {
       req =i;
    //   printf("we have %d and need %d at %d\n",total,req,i);
       if(total < req && a[i]!='0')
       {
          // printf("we needed %d but have only %d\n",req,total);
           ans+=req-total;
           total = req;
       }
     //  printf("adding %d for %c %d %s\n",a[i]-'0',a[i],i,a);
       total+=a[i]-'0';
       
   }
   printf("Case #%d: %d\n",ca,ans);
}

return 0;
}
