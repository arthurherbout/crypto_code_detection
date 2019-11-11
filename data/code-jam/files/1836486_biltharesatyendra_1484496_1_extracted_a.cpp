#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
#include<string>
#include<ctype.h>
using namespace std;
typedef unsigned long long int64;
struct hi{
       int a, b;
       };
       typedef struct hi P;
int cmp(P a,P b)
{
    if(a.a<b.a)return 1;
    return 0;
}
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    for(ca=1;ca<=t;ca++)
    {
                        int n;
                        scanf("%d",&n);
                        int ans[50];
                        int i;
                    map<int,int> gr;
                    vector<P> v;
                        map<int,int>::iterator I;
                        for(i=0;i<n;i++)
                        {ans[i]=0;
                                          
                                          }
                                          int ma=1<<n;
                                        //  printf("ma is %d\n",ma);
                                          int a[50];
                                          for(i=0;i<n;i++)scanf("%d",&a[i]);
                                          int j;
                                          int k;
                                          P tmp;
                                          for(j=0;j<ma;j++)
                                          {
                                                           k=0;
                                                           int x;
                                                           x=j;
                                                           while(1)
                                                           {
                                                                   ans[k++]=x%2;
                                                                   x=x/2;
                                                                   if(x==0)break;
                                                                   }
                                                                   int sum=0;
                                                                   for(i=0;i<n;i++)sum=sum+ans[i]*a[i];
                                                                  // printf("for %dsum is %d\n",j,sum);
                                                                   gr[sum]++;
                                                                tmp.a=sum;
                                                                tmp.b=j;   
                                                                v.push_back(tmp);
                                                           }
                                                           int valid=0;
                                                         int res;
                                                           for(I=gr.begin();I!=gr.end();I++)
                                                           if(I->second>1)
                                                           {valid=1;
                                                           res=I->first;
                                                          // printf("%d comnes %d times\n",res,I->second);
                                                           break;
                                                           }
                                                             printf("Case #%d:\n",ca);
                                                           if(valid){
                                                                   
                                                                     sort(v.begin(),v.end(),cmp);
                                                                     int fir,sec;
                                                          for(i=0;i<v.size();i++)
                                                          {
                                                                                 if(v[i].a==res){//printf("for %d it is \n",v[i].a);
                                                                                                 break;
                                                                                                 }
                                                                                 
                                                                                 }
                                                                                 fir=v[i].b;
                                                                                 sec=v[i+1].b;
                                                                                // printf("ans is %d %d\n",fir,sec);
                                                                                  
                                                                                   for(i=0;i<n;i++)
                        {ans[i]=0;
                                          
                                          }
                                                           int x=fir;
                                                          // printf("for soum %d %d\n",fir,sec);
                                                           int k=0;
                                                           while(1)
                                                           {
                                                                   ans[k++]=x%2;
                                                                   x=x/2;
                                                                   if(x==0)break;
                                                                   }
                                                                  
                                                                     for(i=0;i<n;i++)if(ans[i]){printf("%d",a[i]);break;}
                                                                    for(i++;i<n;i++)if(ans[i])printf(" %d",a[i]);
                                                                   printf("\n");
                                                                      for(i=0;i<n;i++)
                        {ans[i]=0;
                                          
                                          }
                                                           x=sec;
                                                           k=0;
                                                           while(1)
                                                           {
                                                                   ans[k++]=x%2;
                                                                   x=x/2;
                                                                   if(x==0)break;
                                                                   }
                                                                    
                                                                   for(i=0;i<n;i++)if(ans[i]){printf("%d",a[i]);break;}
                                                                    for(i++;i<n;i++)if(ans[i])printf(" %d",a[i]);
                                                                   printf("\n");
                                                                   
                                                                   
                                                           }
                                                           else printf(" Impossible\n");
                                                           
                        }
    return 0;
}
