#include<stdio.h>
#include<vector>
#include<algorithm>
#include<string.h>
#include<math.h>
#include<conio.h>

using namespace std;
int main()
{
    int t;
    
    int po[15];
    po[0]=1;
    int i;
    for(i=1;i<10;i++)po[i]=10*po[i-1];
    int ca,A,B;
    int n;
    int las,fir,m;
    double di;
    vector<int> v[10005];
    for(n=1;n<10005;n++)

   {
 
            
            if(n>1000)
            {
           
            }
           
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
                       //   printf("this is dig arra\n");
                          int j;
                             int l=i;
                          reverse(a,a+l);
                         
                       
                          for(i=0;i<l;i++)
                          {
                                          //printf("starting form %dth %d\n",i,a[i]);
                                          int co;
                                          int val=0;
                                          for(j=i,co=0;co<l;co++)
                                          {
                                                                 val=val*10+a[j];
                                                                 j=(j+1)%l;
                                                                 }
                                                                 
                                                               if(val>n){v[n].push_back(val);
                                                              
                                                                 }
                                                                 
                                          
                                          }  
                               
                               }
                              sort(v[n].begin(),v[n].end());
                              // getch();
                       }
                       scanf("%d",&t);
    for(ca=1;ca<=t;ca++)
    {
                        scanf("%d%d",&A,&B);
                       // printf("looking for %d %d\n",A,B);
                        int ans=0;
                        for(m=A;m<=B;m++)
                        {
                                      
                                         int l=v[m].size();
                                         for(i=0;i<l;i++)
                                         if(v[m][i]>B)break;
                                         else ans++;
                                         
                                         
                                         
                                         }
                        printf("Case #%d: %d\n",ca,ans);
                        
                        }
                        return 0;
}
