#include <iostream>
#include <cstdio>
#include <cmath>

using namespace std;

int gcd(int a, int b)
{
        if(b == 0)
        {
                return a;
        }
        else
        {
                return gcd(b, a % b);
        }
}
int lcm( int a, int b)
{
    return a*b/gcd(a,b);
}




int main()
{
    
    FILE * sisf=fopen("B2.in", "r");
    FILE * valf=fopen("B2.out", "w");
    
    int T; 
    fscanf(sisf, "%d", &T);
    
    for(int t = 1; t<=T; t++)
    {
       int N;
       
       int ar[1010];
       for(int i = 0; i<=1000; i++)ar[i]=0;
       
       fscanf(sisf, "%d", &N);
       //int max=0;
       /*for(int i = 1; i<=N; i++){
          if(ar[i]==0)max++;
          
          for(int j = 2; j<=N; j++)
             if(ar[j]!=0 && j*i <= 1000)ar[j*i]=1;
          ar[i]=1;
       }*/
       int max = 1;
       int min = 1;
       for(int i = 0; i<=1000; i++)ar[i]=0;
       
       /*for(int i = N; i>=1; i--){
          if(ar[i]==0)min++;
          for(int j = 1; j<=N; j++)
             if(ar[j]!=0 && j%i==0){
                min--;
                break;
             }
          ar[i]=1;
          /*if(ar[i]==0)min++;
          
          for(int j = 2; j<=N; j++)
             if(ar[j]!=0 && j*i <= 1000)ar[j*i]=1;
          ar[i]=1;*/
       long long int arv =N;
       
       for(int i = N-1; i>=1; i--){
          if(arv%i!=0){
             arv=lcm(arv,i);
             min++;
          }
       }
       arv = 1;
       for(int i = 2; i<=N; i++){
          if(arv%i!=0){
             arv=lcm(arv,i);
             max++;
          }
       }
       
       fprintf(valf, "Case #%d: %d\n", t,abs( max- min));
    }
    
    
    
    return 0;
}
