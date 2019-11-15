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

struct myrand32{
  unsigned x,y,z,w;

  myrand32(void){x=123456789,y=362436069,z=521288629,w=(unsigned)time(NULL);}
  myrand32(unsigned seed){x=123456789,y=362436069,z=521288629,w=seed;}
  inline unsigned get(void){unsigned t;t=(x^(x<<11));x=y;y=z;z=w;w=(w^(w>>19))^(t^(t>>8));return w;}
  inline double getUni(void){return get()/4294967296.0;}
  inline int get(int a){return (int)(a*getUni());}
  inline int get(int a, int b){return a+(int)((b-a+1)*getUni());}
  inline ll get(ll a){return(ll)(a*getUni());}
  inline ll get(ll a, ll b){return a+(ll)((b-a+1)*getUni());}
  inline double get(double a, double b){return a+(b-a)*getUni();}
  inline int getExp(int a){return(int)(exp(getUni()*log(a+1.0))-1.0);}
  inline int getExp(int a, int b){return a+(int)(exp(getUni()*log((b-a+1)+1.0))-1.0);}
};


int is_same(char a[], char b[], int len){
  int i;
  rep(i,len) if(a[i]!=b[i]) return 0;
  return 1;
}

int N,Q;
int go[1000][1000], gos[1000], tt[1000];
char S[1000];
char SS[1000][1000]; int len[1000];

int st[1000], sts;
int szz[1000];
double sz[1000];
double fact[1000];
int ok[1000];

void get_sz(int n){
  int i, k;
  if(sz[n] > 0) return;

  szz[n] = 1;
  sz[n] = 1;
  rep(i,gos[n]){
    k = go[n][i];
    get_sz(k);
    szz[n] += szz[k];
    sz[n] *= sz[k];
  }
  sz[n] *= szz[n];
}

int main(){
  int i, j, k, a;
  int T, TC = 0;
  int loop;
  double tot, x;
  myrand32 rd;
  double all, res[10];
  char buf[1000];

  fact[0] = 1;
  REP(i,1,1000) fact[i] = fact[i-1] * i;

  reader(&T);
  while(T--){
    fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++TC);

    reader(&N);
    rep(i,N) reader(tt+i), tt[i]--;
    reader(S);
    reader(&Q);
    rep(i,Q) len[i] = reader(SS[i]);

    rep(i,N) gos[i] = 0;
    rep(i,N) if(tt[i]!=-1){
      go[tt[i]][gos[tt[i]]++] = i;
    }

    all = 0;
    rep(i,Q) res[i] = 0;

    rep(i,N) sz[i] = -1;
    rep(i,N) get_sz(i);
    rep(i,N) sz[i] = szz[i];

//    writerArr(sz,N);

    rep(loop,70000){
      k = 0;

      rep(i,N) ok[i] = 1;
      rep(i,N) rep(j,gos[i]) ok[go[i][j]] = 0;
      sts = 0;
      rep(i,N) if(ok[i]) st[sts++] = i;

      while(sts){
        tot = 0;
        rep(i,sts) tot += sz[st[i]];
        x = rd.getUni() * tot;
        rep(i,sts){
          x -= sz[st[i]];
          if(x < 0) break;
        }

//        writerLn(i,a,sts);
        a = st[i];
        sts--;
        REP(j,i,sts) st[j] = st[j+1];
        rep(j,gos[a]) st[sts++] = go[a][j];

        buf[k++] = S[a];
//        printf("%d ",a);
      }
//      puts("");

//      reverse(buf, buf+N);

      rep(i,Q){
        rep(j,N-len[i]+1){
          if(is_same(buf+j,SS[i],len[i])){
            res[i]++;
            break;
          }
        }
      }
      all++;
    }

    rep(i,Q) res[i] /= all;
    writerArr(res,Q);
  }

  return 0;
}
