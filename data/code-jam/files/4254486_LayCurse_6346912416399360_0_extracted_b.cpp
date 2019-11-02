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
int reader(char c[]){int i,s=0;for(;;){mygc(i);if(i!=' '&&i!='\n'&&i!='\r'&&i!='\t'&&i!=EOF) break;}c[s++]=i;for(;;){mygc(i);if(i==' '||i=='\n'||i=='\r'||i=='\t'||i==EOF) break;c[s++]=i;}c[s]='\0';return s;}
template <class T, class S> void reader(T *x, S *y){reader(x);reader(y);}
template <class T, class S, class U> void reader(T *x, S *y, U *z){reader(x);reader(y);reader(z);}
template <class T, class S, class U, class V> void reader(T *x, S *y, U *z, V *w){reader(x);reader(y);reader(z);reader(w);}

void writer(int x, char c){int s=0,m=0;char f[10];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(ll x, char c){int s=0,m=0;char f[20];if(x<0)m=1,x=-x;while(x)f[s++]=x%10,x/=10;if(!s)f[s++]=0;if(m)mypc('-');while(s--)mypc(f[s]+'0');mypc(c);}
void writer(const char c[]){int i;for(i=0;c[i]!='\0';i++)mypc(c[i]);}
void writer(const char x[], char c){int i;for(i=0;x[i]!='\0';i++)mypc(x[i]);mypc(c);}
template<class T> void writerLn(T x){writer(x,'\n');}
template<class T, class S> void writerLn(T x, S y){writer(x,' ');writer(y,'\n');}
template<class T, class S, class U> void writerLn(T x, S y, U z){writer(x,' ');writer(y,' ');writer(z,'\n');}
template<class T> void writerArr(T x[], int n){int i;if(!n){mypc('\n');return;}rep(i,n-1)writer(x[i],' ');writer(x[n-1],'\n');}

template<class T> void sort(int N, T a[], void *mem = NULL){sort(a,a+N);}
template<class T1, class T2> void sort(int N, T1 a[], T2 b[], void *mem){int i;pair<T1,T2> *r=(pair<T1, T2>*)mem;rep(i,N)r[i].first=a[i],r[i].second=b[i];sort(r,r+N);rep(i,N)a[i]=r[i].first,b[i]=r[i].second;}
template<class T1, class T2, class T3> void sort(int N, T1 a[], T2 b[], T3 c[], void *mem){int i;pair<T1,pair<T2,T3> > *r=(pair<T1,pair<T2,T3> >*)mem;rep(i,N)r[i].first=a[i],r[i].second.first=b[i],r[i].second.second=c[i];sort(r,r+N);rep(i,N)a[i]=r[i].first,b[i]=r[i].second.first,c[i]=r[i].second.second;}
template<class T1, class T2, class T3, class T4> void sort(int N, T1 a[], T2 b[], T3 c[], T4 d[], void *mem){int i;pair<pair<T1,T2>,pair<T3,T4> > *r=(pair<pair<T1,T2>,pair<T3,T4> >*)mem;rep(i,N)r[i].first.first=a[i],r[i].first.second=b[i],r[i].second.first=c[i],r[i].second.second=d[i];sort(r,r+N);rep(i,N)a[i]=r[i].first.first,b[i]=r[i].first.second,c[i]=r[i].second.first,d[i]=r[i].second.second;}

#define MD 1000000007

int N, K;
ll A[100000];

ll now[1000];
ll mn[1000], mx[1000];

int main(){
  int i, j, k, st;
  int T, C = 0;
  ll res, tmp, tmx, tmn, sum, tar[200], mm, mz;
  void *mem = malloc(30000000);

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++C);

    reader(&N,&K);
    rep(i,N-K+1) reader(A+i);
    res = 0;

    rep(i,K) now[i] = 0;
    rep(i,K) mn[i] = mx[i] = 0;
    rep(i,N-K){
      j = i%K;
      now[j] += A[i+1] - A[i];
      mn[j] = min(mn[j], now[j]);
      mx[j] = max(mx[j], now[j]);
    }

    mn[0] += A[0];
    mx[0] += A[0];

    sum = 0;
    rep(i,K) sum += mn[i];

    mm = sum / K;
    rep(i,K) tar[i] = mm, sum -= mm;
    rep(i,K){
      if(sum>0) tar[i]++, sum--;
      if(sum<0) tar[i]--, sum++;
    }
    
    rep(i,K){
      mx[i] += tar[i] - mn[i];
      mn[i] += tar[i] - mn[i];
    }

    res = 10000000000000000LL;

    rep(k,12000){
      tmx = mx[0];
      tmn = mn[0];
      rep(i,K) tmx = max(tmx, mx[i]);
      rep(i,K) tmn = min(tmn, mn[i]);
      res = min(res, tmx - tmn);

      sort(K, mn, mx, mem);
      mm = 0;
      rep(i,K) if(mx[mm] > mx[i]) mm = i;
      mz = K-1;
      for(i=K-1;i>=0;i--) if(mx[mz] < mx[i]) mz = i;

      mn[mm]++; mx[mm]++;
      mn[mz]--; mx[mz]--;
    }
    writerLn(res);
  }

  return 0;
}
