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
double pi = acos(-1.0);
int main()
{
 int t,ca;
 scanf("%d",&t);
 //printf("case %d\n",t);
 for(ca=1;ca<=t;ca++)
 {
                     int th;
                     int r;
                     scanf("%d%d",&r,&th);
                    // printf("you gave %d and %.3lf\n",r,th);
                     int ans=0;
                     while(1)
                     {
                             double area = (r+1)*(r+1) - r*r;
                            
                           //  printf("area is %.2lf\n",area);
                             if(area<=th){
                                        // printf("we can made\n");
                                         ans++; 
                                         th-=area;
                                         r+=2;
                                         }
                                         else break;
                             
                             }
                     
                     printf("Case #%d: %d\n",ca,ans);
                     
                     }
                     return 0;   
    
    
}
