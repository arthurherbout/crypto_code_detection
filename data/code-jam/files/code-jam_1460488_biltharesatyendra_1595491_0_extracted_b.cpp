#include<stdio.h>
#include<algorithm>
using namespace std;
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    for(ca=1;ca<=t;ca++)
    {
                        int n,s,p,a[105];
                        int i;
                        scanf("%d%d%d",&n,&s,&p);
                        for(i=0;i<n;i++)scanf("%d",&a[i]);
                        int ans=0;
                        sort(a,a+n);
                        
                       int lo=3*(p-1)-1;
                       int hi=3*(p-1);
                     for(i=0;i<n;i++)
                     if((a[i]==lo)||(a[i]==hi)){
                                                if(s){
                                                      s--;
                                                      if(a[i])
                                                      ans++;
                                                      }
                                                }
                                                else if(a[i]>hi)break;
                                                for(;i<n;i++)ans++;
                                              //  printf("%d is by default\n",a[i]);
                                                
                        printf("Case #%d: %d\n",ca,ans);
                        
                        }
    return 0;
}
