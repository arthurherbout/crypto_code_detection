#include<string.h>
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int a[1000005];
 int n;
 
int sol(int ans)
{
   int st;
   for(st = 0; st<=ans;st++)
   {
      // printf("taking only steps %d\n",st);
       multiset<int> ss ;
       int i;
       for(i=0;i<n;i++)
       ss.insert(a[i]);
       int j;
          multiset<int>::iterator I;
       for(j=0;j<st;j++)
       {
        
           I = ss.end();
           I--;
           int cur = *I;
           ss.erase(I);
          // printf("removed %d and inserted %d %d\n",cur,cur/2,(cur+1)/2);
           ss.insert(cur/2);
           ss.insert((cur+1)/2);
           
       }
       I = ss.end();
       I--;
      // printf("last element is %d\n",*I);
       if(st + *I <=ans)return 1;
       
   }
   return 0;
}
int main()
{
int i,j;
int t,ca;
scanf("%d",&t);
for(ca = 1; ca<=t;ca++)
{
  
   int hi = 0;
   scanf("%d",&n);
   for(i=0;i<n;i++)
   {
       scanf("%d",&a[i]);
      // printf("%d ",a[i]);
       if(a[i] > hi)hi = a[i];
   }
   int lo,ans;
   lo = 0;
   while(lo<=hi)
   {
       ans = (lo+hi)/2;
       int u = sol(ans);
       if(u==0)lo = ans+1;
       else {
           if(sol(ans-1)==0)break;
           hi = ans-1;
       }
   }
   /*
   for(ans = 1; ans<=hi;ans++){
     //  printf("checking for %d\n",ans);
   if(sol(ans)==1)break;
   }
   */
   printf("Case #%d: %d\n",ca,ans);
}

return 0;
}
