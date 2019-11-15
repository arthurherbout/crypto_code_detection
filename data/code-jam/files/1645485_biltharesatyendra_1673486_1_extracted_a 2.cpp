#include<stdio.h>

int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    int a,b;
    for(ca=1;ca<=t;ca++)
    {
                        scanf("%d%d",&a,&b);
                        double val[100005];
                        
                        int i;
                         double pro[100005];
                         pro[0]=1.0f;
                        for(i=1;i<=a;i++){
                        scanf("%lf",&val[i]);
                        pro[i]=val[i]*pro[i-1];
                        }
                        double fi,ba,retry;
                        double ri;
                        double mi=5*b;
                     int rem=b-a;
                     
                    retry=b+2;
                    
                    fi=pro[a]*(1+rem)+(1-pro[a])*(1+rem+b+1);
                    ba=5*b;
                    double ans;
                   
    for(i=1;i<=a;i++)
    {
                     
                     //we are pressing i back spaces 
                     ri=pro[a-i];
                     ans=(2*i+rem+1)*ri+(2*i+rem+b+2)*(1-ri);
                    if(ans<ba)ba=ans;
                    
                     
                     }
                    
                     if(ba<mi)mi=ba;
                     if(retry<mi)mi=retry;
                     if(fi<mi)mi=fi;
              
                     printf("Case #%d: %.6lf\n",ca,mi);
                     
                        }
    return 0;
}
