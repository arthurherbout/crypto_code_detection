#include<stdio.h>
#include<vector>
#include<algorithm>
#include<string.h>
#include<math.h>
#include<conio.h>
#include<list>
using namespace std;
typedef unsigned long long int64;
struct why{
       int a,b;
       };
typedef struct why W;

int cmp(W x,W y)
{
    if(x.a<y.a)return 1;
    else if(x.a==y.a){
         if(x.b<y.b)return 1;
         }
         return 0;
         }
int main()
{
    int t;
    
   
    int po[15];
    po[0]=1;
    int i;
    int j;
   
  
    for(i=1;i<10;i++)po[i]=10*po[i-1];
    int ca,A,B;
    int n;
    int las,fir,m;
    double di;
    vector<int> v[10];

      
       for(n=0;n<2000005;n++)
       {
                             v[0].push_back(-1);
                           v[1].push_back(-1);
                             v[2].push_back(-1);
                               v[3].push_back(-1);
                                 v[4].push_back(-1);
                                   v[5].push_back(-1);
                                     v[6].push_back(-1);
                                       v[7].push_back(-1);
                                       v[8].push_back(-1);
                                       }
 for(n=1;n<=2000005;n++)

   {

          
          
           
         
                   m=n;
                      
                      int i=0;
                     int a[500];
                      while(1)
                      {
                          a[i++]= m%10;
                          m/=10;
                          if(m==0)break;
                          }
                         // printf("this is dig arra\n");
                          
                          int j;
                         
                             int l=i;
                             
                          reverse(a,a+l);
                          // for(j=0;j<l;j++)
                        // printf("%d ",a[j]);
                          //printf("\n");
                       
                       
                          for(i=0;i<=l;i++)
                          {
                                          //printf("starting form %dth %d\n",i,a[i]);
                                          int co;
                                          int val=0;
                                          for(j=i,co=0;co<l;co++)
                                          {
                                                                 j=j%l;
                                                                 val=val*10+a[j];
                                                                 j=(j+1)%l;
                                                                 }
                                                               //  printf("%d\n",val);
                                                               if(val>n){
                                                                         double fir,sec;
                                                                         fir=log10(val);
                                                                         sec=log10(n);
                                                                         int u,w;
                                                                         u=(int)fir;
                                                                         w=(int)sec;
                                                                         if(u==w)
                                                                         v[i][n]=val;
                                                             //  printf("%d\n",val);
                                                              
                                                                 }
                                                                // else break;
                                                            
                                                           // printf("\n");   
                                          
                                          }  
                                           // for(i=0;i<8;i++)
                                                            //  printf("%d ",v[i][n]);
                                                            //  printf("\n");
                               
                               
                             
                              // getch();
                       }
                      // printf("done\n");
                       scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
    {
                        int ne=0;
                        scanf("%d%d",&A,&B);
                        int hal;
                        if(A>B){
                                hal=A;A=B;B=hal;
                                }
                       // printf("looking for %d %d\n",A,B);
                        int64 ans=0;
                        int total;
                        vector<W> ho;
         W tmp;
                        for(m=A;m<=B;m++)
                        {  list<int> li;
                                        
                                    
                                         for(i=0;i<=8;i++){
                                         if(v[i][m]<=B&&v[i][m]>=A&&v[i][m]>m)
                                         
                                         {
                                                                              
                                       
                                                              li.push_back(v[i][m]);
                                         
                                         }
                                         }
                                      
                                 
                                      li.sort();
                                      
                                                                              li.unique();
                                                                              ans+=li.size();   
                                     
                                         
                                         }
                                      

int i;
/*
for(i=0;i<si;i++)
printf("%d %d\n",ho[i].a,ho[i].b);
*/
                        printf("Case #%d: %llu\n",ca,ans);
                        
                        }
                        return 0;
}
