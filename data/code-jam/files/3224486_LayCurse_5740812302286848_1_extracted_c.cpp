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
#define EPS 1e-10

template <class T>
struct DijkstraHeap {
  int *hp, *place, size; char *visited; T *val;

  void malloc(int N){hp=(int*)std::malloc(N*sizeof(int));place=(int*)std::malloc(N*sizeof(int));visited=(char*)std::malloc(N*sizeof(char));val=(T*)std::malloc(N*sizeof(T));}
  void free(){free(hp);free(place);free(visited);free(val);}
  void* malloc(int N, void *workMemory){hp=(int*)workMemory;place=(int*)(hp+N);visited=(char*)(place+N);val=(T*)(visited+N);workMemory=(void*)(val+N);return workMemory;}
  void init(int N){int i;size=0;rep(i,N)place[i]=-1,visited[i]=0;}
  void up(int n){int m;while(n){m=(n-1)/2;if(val[hp[m]]<=val[hp[n]])break;swap(hp[m],hp[n]);swap(place[hp[m]],place[hp[n]]);n=m;}}
  void down(int n){int m;for(;;){m=2*n+1;if(m>=size)break;if(m+1<size&&val[hp[m]]>val[hp[m+1]])m++;if(val[hp[m]]>=val[hp[n]]) break;swap(hp[m],hp[n]);swap(place[hp[m]],place[hp[n]]);n=m;}}
  void change(int n, T v){if(visited[n]||(place[n]>=0&&val[n]<=v))return;val[n]=v;if(place[n]==-1)place[n]=size,hp[size++]=n,up(place[n]);else up(place[n]);}
  int pop(void){int res=hp[0];place[res]=-1;size--;if(size)hp[0]=hp[size],place[hp[0]]=0,down(0);visited[res]=1;return res;}
};


int N, S;
int X[1000], Y[1000], Z[1000], VX[1000], VY[1000], VZ[1000];

double tm1[1000][1000], tm2[1000][1000];
int reach[1000][1000];
int st[1000000], sts;

void gettimes(int x, int y, int z, int vx, int vy, int vz, double c,double *t1, double *t2){
  double s, t, u;

  s = vx*vx + vy*vy + vz*vz;
  t = 2*(x*vx + y*vy + z*vz);
  u = x*x + y*y + z*z - c*c;

  if(s < EPS){
    if(u <= EPS){
      *t1 = 0;
      *t2 = 1e100;
    } else {
      *t1 = *t2 = 2e100;
    }
    return;
  }

  if(t*t - 4*s*u < 0){
    *t1 = *t2 = 1e100;
    return;
  }

  *t1 = (-t - sqrt(t*t-4*s*u)) / (2*s);
  *t2 = (-t + sqrt(t*t-4*s*u)) / (2*s);
  if((*t1) < 0) *t1 = 0;
  if((*t2) < 0) *t1 = *t2 = 2e100;
}

int main(){
  int i, j, k;
  int s, t, u;
  int T, TC = 0;
  double a, b, c;
  double t1,t2;
  int ok;
  DijkstraHeap<double> hp;

  hp.malloc(1000000);

  reader(&T);
  while(T--){
    fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++TC);

    reader(&N,&S);
    rep(i,N){
      reader(X+i,Y+i,Z+i);
      reader(VX+i,VY+i,VZ+i);
    }

    a = 0;
    b = 10000;
    while(b-a > 1e-5){
      c = (a+b) / 2;

      rep(i,N) REP(j,i+1,N){
        gettimes(X[i]-X[j],Y[i]-Y[j],Z[i]-Z[j],VX[i]-VX[j],VY[i]-VY[j],VZ[i]-VZ[j],c,&t1,&t2);
        tm1[i][j] = tm1[j][i] = t1;
        tm2[i][j] = tm2[j][i] = t2;
      }

      hp.init(N*N);
      ok = 0;
      
      rep(i,N) if(i!=0) if(tm1[0][i] <= S+EPS){
        hp.change(i, 0.0);
      }

      while(hp.size){
        k = hp.pop();
        i = k / N;
        j = k % N;

        if(i==1||j==1){ok = 1; break;}

        rep(s, N){
          if(tm1[j][s] > tm2[i][j]+S+EPS) continue;
          if(tm2[j][s] < hp.val[k]-EPS) continue;
          hp.change(j*N+s, max(hp.val[k], tm1[j][s]));
        }
      }

      if(ok) b=c; else a=c;
    }

    writerLn((a+b)/2);
  }

  return 0;
}
