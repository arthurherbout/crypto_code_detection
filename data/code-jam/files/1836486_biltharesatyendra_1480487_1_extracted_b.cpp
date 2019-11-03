#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<vector>
#include<queue>
#include<map>
#include<string>
#include<ctype.h>
typedef unsigned long long int64;
using namespace std;

int gcd(int a,int b)
{
    int t;
    while(a%b){
               t=a%b;
               a=b;
               b=t;
               }
               return b;
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
                        int a[500];
                        int i;
                        for(i=0;i<n;i++)
                        scanf("%d",&a[i]);
                        int sum=0;
                        int sum1=0;
                        for(i=0;i<n;i++)sum+=a[i];
                        double avg=(double)sum/double(n);
                        double pr[500];
                        double total=0.0;
                        int fir[1005],sec[1005];
                        //printf("sum is %d\n",sum);
                        int count=0;
                        int s;
                        s=sum;
                        for(i=0;i<n;i++){
                     fir[i]=100*(2*sum-n*a[i]);
                     if(fir[i]<=0){fir[i]=0;count++;sum1+=a[i];}

                    
                    // pr[i]=pr[i]+er;
                     }
                   //  printf("there are %d which need not vote\n",count);
                     n=n-count;
                     sum=2*sum-sum1;
                     for(i=0;i<n+count;i++)
                     {
                               //    printf("%d ",fir[i]);
                     if(fir[i]==0)pr[i]=0;
                     else{
                      fir[i]=100*(sum-n*a[i]);
                //    printf("fir is %d\n",fir[i]);
                       sec[i]=(n)*s;
             //       printf("sec i is %d\n",sec[i]);
                       int g=gcd(fir[i],sec[i]);
                       fir[i]/=g;
                       sec[i]/=g;
                       pr[i]=(double)fir[i]/double(sec[i]);
                       }
                                     }
                     
                        printf("Case #%d:",ca);
                        for(i=0;i<n+count;i++)
                        printf(" %.6lf",pr[i]);
                        printf("\n");
                        }
    return 0;
}
