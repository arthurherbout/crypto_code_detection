#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define mygc(c) (c)=getchar_unlocked()
#define mypc(c) putchar_unlocked(c)

#define ll long long
#define ull unsigned ll

void reader(int *x){int k,m=0;*x=0;for(;;){mygc(k);if(k=='-'){m=1;break;}if('0'<=k&&k<='9'){*x=k-'0';break;}}for(;;){mygc(k);if(k<'0'||k>'9')break;*x=(*x)*10+k-'0';}if(m)(*x)=-(*x);}
void reader(ll *x){int k,m=0;*x=0;for(;;){mygc(k);if(k=='-'){m=1;break;}if('0'<=k&&k<='9'){*x=k-'0';break;}}for(;;){mygc(k);if(k<'0'||k>'9')break;*x=(*x)*10+k-'0';}if(m)(*x)=-(*x);}
void reader(double *x){scanf("%lf",x);}
int reader(char c[]){int i,s=0;for(;;){mygc(i);if(i!=' '&&i!='\n'&&i!='\r'&&i!='\t'&&i!=EOF) break;}c[s++]=i;for(;;){mygc(i);if(i==' '||i=='\n'||i=='\r'||i=='\t'||i==EOF) break;c[s++]=i;}c[s]='\0';return s;}
template <class T, class S> void reader(T *x, S *y){reader(x);reader(y);}
template <class T, class S, class U> void reader(T *x, S *y, U *z){reader(x);reader(y);reader(z);}
template <class T, class S, class U, class V> void reader(T *x, S *y, U *z, V *w){reader(x);reader(y);reader(z);reader(w);}

void writer(int x, char c){int s=0,m=0;char f[10];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(ll x, char c){int s=0,m=0;char f[20];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(double x, char c){printf("%.15f",x);mypc(c);}
void writer(const char c[]){int i;for(i=0;c[i]!='\0';i++)mypc(c[i]);}
void writer(const char x[], char c){int i;for(i=0;x[i]!='\0';i++)mypc(x[i]);mypc(c);}
template<class T> void writerLn(T x){writer(x,'\n');}
template<class T, class S> void writerLn(T x, S y){writer(x,' ');writer(y,'\n');}
template<class T, class S, class U> void writerLn(T x, S y, U z){writer(x,' ');writer(y,' ');writer(z,'\n');}
template<class T> void writerArr(T x[], int n){int i;if(!n){mypc('\n');return;}rep(i,n-1)writer(x[i],' ');writer(x[n-1],'\n');}

char memarr[17000000]; void *mem = memarr;
#define MD 1000000007

int N, A[1111];
int edge[1111][1111], es[1111];
int vis[1111];

int solve(int now, int a, int b){
  int i, j, k;
  int res = 1;
  int mx = 0, tmp;

  rep(i,es[now]){
    k = edge[now][i];
    if(k==a || k==b) continue;
    tmp = solve(k, a, b);
    mx = max(mx, tmp);
  }

  res += mx;
  return res;
}

int main(){
  int i, j, k;
  int T, TC = 0;
  int res, tmp;
  int add;

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++TC);

    reader(&N);
    rep(i,N) reader(A+i), A[i]--;
    res = 0;

    rep(i,N) es[i] = 0;
    rep(i,N){
      j = A[i];
      edge[j][es[j]++] = i;
    }

    rep(k,N){
      rep(i,N) vis[i] = 0;
      tmp = 0;
      i = k;
      while(vis[i]==0){
        vis[i] = 1; tmp++; i = A[i];
      }
      if(i!=k) continue;
      res = max(res, tmp);
    }

    tmp = 0;
    rep(i,N) REP(j,i+1,N) if(A[i]==j && A[j]==i) tmp += solve(i,i,j) + solve(j,i,j);
    res = max(res, tmp);

    writerLn(res);
  }

  return 0;
}
