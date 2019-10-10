#include<stdio.h>
int main()
{
    int t;
    scanf("%d",&t);
    int ca;
    for(ca=1;ca<=t;ca++)
    {
                        int n;
                        int i;
                        scanf("%d",&n);
                        int rsm[15],rlar[15],vism[15],vilar[15];
                        for(i=0;i<=n;i++)vism[i]=vilar[i]=0;
                    int j;
                        for(i=1;i<=n;i++)
                        {
                                        scanf("%d%d",&rsm[i],&rlar[i]);
                                        }
                                        int done=0;
                                        int po=0;
                                        int found;
                                        int st=0;
                                        int fi=0;
                                        while(1)
                                        {
                                                found=0;
                                                for(i=1;i<=n;i++)
                                                if(vilar[i]==0){
                                                               
                                                                if(rlar[i]<=po){
                                                                                 
                                                                             vilar[i]=1;
                                                                              if(vism[i]){
                                                                                          
                                                                                          for(j=1;j<=n;j++)
                                                                                          if((vism[j]==1)&&(rsm[j]<=rsm[i]))break;
                                                                                          
                                                                                          if(j==n)
                                                                                          po+=1;
                                                                                          else {
                                                                                               vism[i]=0;
                                                                                               vism[j]=1;
                                                                                               po+=2;
                                                                                               }
                                                                                               
                                                                                               
                                                                                          
                                                                              }
                                                                              else po+=2;
                                                                              found=1;
                                                                              fi++;
                                                                             break;
                                                                               }
                                                                }
                                                                if(found);
                                                                else {
                                                                     for(i=1;i<=n;i++)
                                                                     if(vism[i]==0){
                                                                                    if(rsm[i]<=po){
                                                                                                  if(vilar[i]);
                                                                                                  else {
                                                                                                  vism[i]=1;
                                                                                                  po+=1;
                                                                                                  found=1;
                                                                                                  break;
                                                                                                  }
                                                                                                  }
                                                                                    }
                                                                     }
                                                                     
                                                                    
                                                                     
                                                                     if(found==0)break;
                                                                      st++;
                                                                     if(fi==n)break;
                                                                     
                                                                     
                                                                     
                                                
                                                }
                                                if(fi==n)
                                                printf("Case #%d: %d\n",ca,st);
                                                else
                                                printf("Case #%d: Too Bad\n",ca);
                                                
                        }
    return 0;
}
