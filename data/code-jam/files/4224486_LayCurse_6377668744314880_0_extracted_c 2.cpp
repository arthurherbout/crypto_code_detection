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

#define EPS 1e-9
#define PI 3.141592653589793238462
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
typedef struct struct_point{double x,y;}pnt;
typedef struct struct_line{pnt a,b;}line;
typedef struct struct_circle{pnt c; double r;}circle;

int doubleSign(double a){if(a>EPS) return 1; if(a<-EPS) return -1; return 0;}
int doubleSignR(double a,double b){ if(doubleSign(b)!=0 && doubleSign(a/b-1)==0) return 0; return doubleSign(a-b);}
int pntSign(pnt a){int i=doubleSign(a.x); if(i) return i; return doubleSign(a.y);}

pnt pntPlus(pnt a,pnt b){a.x+=b.x; a.y+=b.y; return a;}
pnt pntMinus(pnt a,pnt b){a.x-=b.x; a.y-=b.y; return a;}
pnt pntMultiple(pnt a,pnt b){pnt c; c.x=a.x*b.x-a.y*b.y; c.y=a.x*b.y+a.y*b.x; return c;}
pnt pntMultipleDouble(pnt a,double k){a.x*=k; a.y*=k; return a;}

int pntIsEqual(pnt a,pnt b){return pntSign(pntMinus(a,b))==0;}

double pntLength(pnt a){return sqrt(a.x*a.x+a.y*a.y);}
double pntLength2(pnt a){return a.x*a.x+a.y*a.y;}
double pntDistance(pnt a,pnt b){return pntLength(pntMinus(a,b));}
double pntDistance2(pnt a,pnt b){return pntLength2(pntMinus(a,b));}
double pntArgument(pnt a){return atan2(a.y,a.x);}

pnt pntNormalize(pnt a){double n=pntLength(a); a.x/=n; a.y/=n; return a;}
double pntInnerProduct(pnt a,pnt b){return a.x*b.x+a.y*b.y;}
double pntOuterProduct(pnt a,pnt b){return a.x*b.y-a.y*b.x;}

pnt pntGenerator(double x,double y){pnt res; res.x=x; res.y=y; return res;}
line pntToLine(pnt a,pnt b){line res; res.a=a; res.b=b; return res;}
circle pntToCircle(pnt c,double r){circle res; res.c=c; res.r=r; return res;}

int isPntInCircle(pnt p,circle c){return doubleSign(c.r-pntDistance(p,c.c))+1;}
int isPntOnCircle(pnt p,circle c){if(doubleSign(pntDistance(p,c.c)-c.r)==0) return 1; return 0;}

/* aがbの厳密に内部なら2 */
int isCircleInCircle(circle a,circle b){return doubleSign(b.r-a.r-pntDistance(a.c,b.c))+1;}

pnt pntPolar(double r,double t){pnt a; a.x=r*cos(t); a.y=r*sin(t); return a;}
void pntSort(pnt d[],int s){int i,j;pnt k,t;if(s<=1)return;k=pntMultipleDouble(pntPlus(d[0],d[s-1]),1/2.0);i=-1;j=s;for(;;){while(pntSign(pntMinus(d[++i],k))==-1);while(pntSign(pntMinus(d[--j],k))==1);if(i>=j)break;t=d[i];d[i]=d[j];d[j]=t;}pntSort(d,i);pntSort(d+j+1,s-j-1);}

/* 点の進行方向 */
/* 1,-1は符号付き面積の符号と同じ */
/* 2 : c -- a -- b */
/* -2: a -- b -- c */
/* 0 : その他      */
int pntCCW(pnt a,pnt b,pnt c){
  double tmp;
  b=pntMinus(b,a); c=pntMinus(c,a);
  tmp = pntOuterProduct(b,c);
  if( doubleSign(tmp) ) return doubleSign( tmp );
  if( doubleSign( pntInnerProduct(b,c) ) == -1 ) return 2;
  if( doubleSign( pntLength(c)-pntLength(b) ) == 1 ) return -2;
  return 0;
}

/* convex hull (Andrew's Monotone Chain) */
/* 制約: size >= 3, pはsorted (x座標について) */
/* return 凸包の頂点数 */
/* たぶん，直線上の中点を含まない → pntCCW(res[res_size-2],res[res_size-1],p[i])<=0 */
/* たぶん，直線上の中点を含める   → pntCCW(res[res_size-2],res[res_size-1],p[i])==-1 */
int pntConvexHull(pnt p[],int size,pnt res[]){
  int i,res_size=0,t;
  for(i=0;i<size;res[res_size++]=p[i++])    while(res_size>=2 && pntCCW(res[res_size-2],res[res_size-1],p[i])==-1) res_size--;
  t=res_size;
  for(i=size-2;i>=0;res[res_size++]=p[i--]) while(res_size> t && pntCCW(res[res_size-2],res[res_size-1],p[i])==-1) res_size--;
  return res_size-1;
}

int N;
pnt p[100]; int res[100];
pnt cnv[100]; int sz;
pnt tmp[100];

int main(){
  int i, j, k, mask;
  int cost;
  int T, C = 0;

  reader(&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d:\n", ++C);

    reader(&N);
    rep(i,N){
      reader(&j,&k);
      p[i]=pntGenerator(j,k);
    }
    rep(i,N) res[i] = N;

    rep(mask,1<<N){
      cost = 0;
      rep(i,N) if(!(mask & 1<<i)) cost++;
      sz = 0;
      rep(i,N) if(mask & 1<<i) cnv[sz++] = p[i];
      if(sz > 3){
        pntSort(cnv, sz);
        cnv[sz] = cnv[0];
        sz = pntConvexHull(cnv, sz, tmp);
        rep(i,sz) cnv[i] = tmp[i];
      }
      rep(i,sz) rep(j,N) if(pntIsEqual(cnv[i],p[j])) res[j] = min(res[j], cost);
    }

    rep(i,N) writerLn(res[i]);
  }

  return 0;
}
