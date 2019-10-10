#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define INF 1000000000

int n, ss;
char buf[10000];
char in[200][200], tmp[200][200];

int ab(int x){if(x<0)return -x; return x;}

void rot(void){
  int i,j;
  rep(i,ss) rep(j,ss) tmp[i][j]=in[i][j];
  rep(i,ss) rep(j,ss) in[i][j]=tmp[j][ss-1-i];
}

int cnt_biggest(void){
  int i,j,k,st,res=0,dame;
  int ii,jj,dist;
  int x1, x2, y1, y2;

  rep(st,n){
    REP(k,1,n+1-st){
      if(res >= k) continue;
      x1 = 0;
      x2 = 2*(k-1);
      y1 = n-1 - (k-1);
      y2 = n-1 + (k-1);

      x1 += st; x2 += st;
      y1 += st; y2 += st;
      
      dame=0;
      REP(i,x1,x2+1) REP(j,y1,y2+1){
        dist = ab((x1+x2)/2 - i) + ab((y1+y2)/2 - j);
        if(dist >= k) continue;

        ii = x2 - (i-x1);
        jj = y2 - (j-y1);

        if(in[i][j]!=in[ii][j]) dame++;
        if(in[i][j]!=in[i][jj]) dame++;
      }

      if(dame==0) res=k;
    }
  }

  return res;
}

int main(){
  int i,j,k,l,m,loop;
  int size, count=0;
  int res;

  gets(buf); size=atoi(buf);
  while(size--){
    fprintf(stderr,"%d\n",size);
    gets(buf); n=atoi(buf);
    ss=2*n-1;
    rep(i,ss) rep(j,ss) in[i][j]=' ';
    rep(i,ss) gets(in[i]);
    rep(i,ss) rep(j,ss) if(in[i][j]<' ') in[i][j]=' ';

    res=0;
    rep(loop,4){
      k = cnt_biggest();
      if(res < k) res = k;
      rot();
    }

    res = (n+n-res) * (n+n-res) - n * n;
    printf("Case #%d: %d\n",++count,res);
  }

  return 0;
}
