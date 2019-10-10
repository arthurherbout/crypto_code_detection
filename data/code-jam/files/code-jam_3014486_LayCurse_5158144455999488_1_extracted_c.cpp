#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

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
  int pop(void){int res=hp[0];place[res]=-1;hp[0]=hp[--size];place[hp[0]]=0;down(0);visited[res]=1;return res;}
};


int N, X, Y;
int xa[2000], xb[2000], ya[2000], yb[2000];
int dist[2000][2000];

int get_dist(int a, int b, int c, int d){
  if(b < c) return c - b;
  if(d < a) return a - d;
  return 0;
}

int main(){
  int i, j, k;
  int T, cnt = 0;
  int res;
  void *mem = malloc(60000000);
  DijkstraHeap<int> heap;

  heap.malloc(20000);
  
  scanf("%d",&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d%d%d",&X,&Y,&N);
    rep(i,N) scanf("%d%d%d%d",xa+i,ya+i,xb+i,yb+i);

    heap.init(N+2);
    rep(i,N+2) dist[i][i] = 0;
    rep(i,N) REP(j,i+1,N){
      dist[i][j] = dist[j][i] = max( get_dist(xa[i],xb[i]+1,xa[j],xb[j]+1), get_dist(ya[i],yb[i]+1,ya[j],yb[j]+1) );
    }
    rep(i,N) dist[i][N] = dist[N][i] = xa[i];
    rep(i,N) dist[i][N+1] = dist[N+1][i] = X-xb[i]-1;
    dist[N][N+1]=dist[N+1][N] = X;

    heap.change(N, 0);
    while(heap.size){
      k = heap.pop();
      rep(i,N+2) heap.change(i, heap.val[k]+dist[k][i]);
    }
    res = heap.val[N+1];

    printf("%d\n",res);
  }

  return 0;
}
