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
#include<set>
using namespace std;
typedef unsigned long long int64;
set<int> mul(vector<int> a)
{
            int n= a.size();
            int ma = 1<<n;
            int cur;
set<int> ans;
            for(cur=0;cur<ma;cur++)
            {
                                   int val=1;
                                   int i;
                                   for(i=0;i<n;i++)
                                   if(cur & (1<<i))val*=a[i];
                                   ans.insert(val);
                                   
                                   }
            return ans;
            }
 int r,n,m,k;
 int a[105];
int sol(set<int> s)
{
  
    int i;
   // printf("we are looking for \n");
    for(i=0;i<k;i++){
                    // printf("%d ",a[i]);
    if(s.find(a[i])==s.end())return 0;
    
}
    return 1;
}
int main()
{

  char b[50];
          int hi = 64;
          int s;
            map<string,set<int> > gr;           
          for(s=0;s<hi;s++)
          {
                           int in=0;
                           for(in=0;in<3;in++)b[in] = '2';
                           in=0;
                           int cur = s;
                           while(cur)
                           {
                                     b[in++] = cur%4 + '2';
                                     cur/=4;
                                     }
                                    // printf("we choose %s\n",b);
                                    vector<int> w;
                                     for(in=0;b[in];in++)
                                     w.push_back(b[in] -'0');
                                     gr[b] = mul(w);
                                     
                    
                    
                           }
                          // printf("done\n");
                            int t,ca;
  
 scanf("%d",&t);
// printf("t is %d\n",t);
             printf("Case #1:\n");       
                     
  
          
          scanf("%d%d%d%d",&r,&n,&m,&k);
        //  printf("values are %d %d %d %d\n",r,n,m,k);
          for(ca=1;ca<=r;ca++){
                              // printf("round %d\n",ca);
                               int i;
                               for(i=0;i<k;i++)
                               {
                                               scanf("%d",&a[i]);
                              // printf("solve for %d\n",a[i]);
                               }
          
         
          
          map< string,set<int> >::iterator I;
          for(I=gr.begin();I!=gr.end();I++)
          {
                               if( (I->first).length() && sol(I->second) ){
                                                  cout<<I->first<<endl;
                                                  break;
                                           }            
                                           }
                     
                     }
                     
                     return 0;   
    
    
}
