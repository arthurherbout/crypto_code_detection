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
void writer(double x, char c){printf("%.15f",x);mypc(c);}
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

int N;
int Y;
int P[600], S[600];

int PP[600], PS[600], pos;
int NP[600], NS[600], neg;

double dp[2][600][600], place[2][600][600];

int main(){
  int i, j, k, m, loop;
  int T, C = 0;
  double res, t, my, ene, tmax, dist;
  void *mem = malloc(30000000);

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++C);

    reader(&Y,&N);
    rep(i,N) reader(P+i);
    rep(i,N) reader(S+i);

    if(0 && N <= 10){
      res = 1e100;
      int ind[100];
      rep(i,N) ind[i] = i;
      do{
        my = 0;
        t = 0;
        rep(k,N){
          i = ind[k];
          if(P[i] > 0) dist = P[i] + t * S[i] - my;
          else         dist = P[i] - t * S[i] - my;
          if(dist > 0) t += dist / (Y-S[i]), my += dist / (Y-S[i]) * Y;
          else         t += (-dist) / (Y-S[i]), my -= (-dist) / (Y-S[i]) * Y;
          //        printf("%f\n",t);
        }
        //      printf("%f ",t); writerArr(ind,N);
        res = min(res, t);
      }while(next_permutation(ind,ind+N));
      printf("[%f] ",res);
    }

    pos = neg = 0;
    rep(i,N){
      if(P[i] > 0){
        PP[pos] = P[i];
        PS[pos] = -S[i];
        pos++;
      } else {
        NP[neg] = -P[i];
        NS[neg] = -S[i];
        neg++;
      }
    }

    sort(pos, PS, PP, mem); rep(i,pos) PS[i] *= -1;
    sort(neg, NS, NP, mem); rep(i,neg) NS[i] *= -1;
    res = 1e100;

    rep(loop,1000){

    rep(i,2) rep(j,pos+1) rep(k,neg+1) dp[i][j][k] = 1e100;
    dp[0][0][0] = dp[1][0][0] = 0;
    place[0][0][0] = place[1][0][0] = 0;

    rep(j,pos+1) rep(k,neg+1) rep(i,2){
//      printf("%d %d %d %f\n",i,j,k,dp[i][j][k]);
      if(j==pos && k==neg){
        res = min(res, dp[i][j][k]);
        continue;
      }

      if(dp[i][j][k] > 1e90) continue;

      t = dp[i][j][k];
      my = place[i][j][k];
/*      if(i==0){
        if(j==0) my = 0; else my = PP[j-1] + PS[j-1] * t;
      } else {
        if(k==0) my = 0; else my = -(NP[k-1] + NS[k-1] * t);
      }*/

      tmax = 0;
      REP(m,j+1,pos+1){
        ene = PP[m-1] + PS[m-1] * t;
        dist = max(0.0, ene - my);
        tmax = max(tmax, dist / (Y-PS[m-1]));
        if(dp[0][m][k] > t + tmax){
          dp[0][m][k] = t + tmax;
          place[0][m][k] = my + tmax * Y;
        }
      }

      tmax = 0;
      REP(m,k+1,neg+1){
        ene = -(NP[m-1] + NS[m-1] * t);
        dist = max(0.0, my - ene);
        tmax = max(tmax, dist / (Y-NS[m-1]));
        if(dp[1][j][m] > t + tmax){
          dp[1][j][m] = t + tmax;
          place[1][j][m] = my - tmax * Y;
        }
      }
    }

      static int ind[600];
      rep(i,pos) ind[i] = rand();
      sort(pos, ind, PP, PS, mem);
      rep(i,neg) ind[i] = rand();
      sort(neg, ind, NP, NS, mem);

    }

    writerLn(res);
  }

  return 0;
}
