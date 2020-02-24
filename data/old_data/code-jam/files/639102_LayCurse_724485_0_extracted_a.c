#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long

int getPrime(int n,int p[]){int i,j,n2=n/2;rep(i,n2)p[i]=1;for(i=3;i*i<n;i+=2)if(p[i>>1])for(j=(i*i)>>1;j<n2;j+=i)p[j]=0;j=1;p[0]=2;REP(i,1,n2)if(p[i])p[j++]=i*2+1;return j;}

int ps, p[2000000];
int pw[10];

int main(){
  int i,j,k,l,m,n,D, pp, end;
  int a;
  int res;
  int size, count=0;
  ll in[120];

  ps = getPrime(2000000,p);
  pw[0]=1; REP(i,1,10) pw[i]=pw[i-1]*10;

  scanf("%d",&size);
  while(size--){
    fprintf(stderr,"rest %d\n",size);
    scanf("%d%d",&D,&n);
    rep(i,n) scanf("%lld",in+i);
    res = -1;
    D = pw[D];

    end = 0;
    if(n>1) rep(i,ps){
      pp = p[i];
      if(pp > D) break;

      rep(k,pp){
        m = (in[1] - in[0] * k)%pp;
        if(m<0) m += pp;
        REP(a,1,n) if((in[a-1]*k+m)%pp != in[a]) break;
        if(a<n) continue;
        a = (in[n-1]*k+m)%pp;
        if(res==-1){res=a; continue;}
        if(res==a) continue;
        end = 1; res = -1; break;
      }
      if(end) break;
    }

    printf("Case #%d: ",++count);
    if(res<0) puts("I don\'t know.");
    else printf("%d\n",res);
  }


  return 0;
}
