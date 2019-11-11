#include<stdio.h>
#include<vector>
#include<string>
#include<queue>
#include<map>
#include<algorithm>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<ctype.h>
using namespace std;
typedef unsigned long long int64;

int main()
{
 int t,ca;
 scanf("%d",&t);
 for(ca=1;ca<=t;ca++)
 {
                     int e,r,n;
                     scanf("%d%d%d",&e,&r,&n);
                     int po=1;
                     int i;
                     for(i=0;i<n;i++)po=po*(e+1);
                  int valid;
                  int ma=0;
                  int a[105];
                  for(i=0;i<n;i++)scanf("%d",&a[i]);
                 // printf("we will check till %d\n",po);
                  for(i=0;i<po;i++)
                  {
                                  // printf("we look for %d\n",i);
                          int dis[10];
                          int j=0;
                          int cur=i;
                          for(j=0;j<n;j++)dis[j] = 0;
                          j=0;
                          while(cur){
                                     dis[j++] = cur%(e+1); cur/=(e+1);}         
                                   
                           // printf("we will try to distribute in this way\n");
                         /*   for(j=0;j<n;j++)
                            printf("%d ",dis[j]);
                            printf("\n");
                            */
                            valid=1;
                            int res=0;
                            int st=e;
                            for(j=0;j<n;j++)
                            {
                                            if(st<dis[j]){valid=0; break;}
                                            res=res+dis[j] * a[j];
                                            st=st-dis[j]+r;
                                            if(st>e)st=e;
                                            }
                                            if(valid){
                                                   //   printf("we can distribute\n");
                                                      if(res>ma)ma=res;
                                                      }
                                   
                                   }  
                    
                     printf("Case #%d: %d\n",ca,ma);
                     
                     
                     
                     }
                     return 0;   
    
    
}
