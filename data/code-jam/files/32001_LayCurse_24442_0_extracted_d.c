#include<stdio.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

void intReverse(int d[],int size){
  int a=0, b=size-1, t;
  while(a<b) t=d[a], d[a]=d[b], d[b]=t, a++, b--;
}

int intNextPermutation(int d[],int size){
  int i,j,k;

  for(k=size-2;k>=0;k--) if(d[k]<d[k+1]) break;
  if(k<0){intReverse(d,size); return 0;}

  for(i=size-1;;i--) if(d[i]>d[k]) break;
  j=d[i], d[i]=d[k], d[k]=j;
  intReverse(d+k+1, size-k-1);
  
  return 1;
}

int main(){
  int i,j,k,l,m,n,len;
  int opt,tmp;
  char in[2000],next[2000];
  int d[100];
  int size,count=0;

  scanf("%d",&size);
  while(size--){
    scanf("%d%s",&k,in);
    for(i=0;;i++) if(in[i]<' ') break; len=i;

    opt = len;
    rep(i,k) d[i]=i;
    do{
      rep(i,len/k) rep(j,k) next[i*k+j]=in[i*k+d[j]];
      tmp=1; REP(i,1,len) if(next[i-1]!=next[i]) tmp++;
      if(opt>tmp) opt=tmp;
    }while(intNextPermutation(d,k));

    printf("Case #%d: %d\n",++count,opt);
  }

  return 0;
}
