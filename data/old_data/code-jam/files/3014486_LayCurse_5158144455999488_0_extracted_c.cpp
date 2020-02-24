#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

void intSort(int d[],int s){
  int i,j; int key,t;
  if(s<=1) return;

  key=(d[0]+d[s-1])/2; i=-1; j=s;
  for(;;){
    while(d[++i] < key); while(d[--j] > key);
    if(i>=j) break; t=d[i]; d[i]=d[j]; d[j]=t;
  }
  intSort(d,i); intSort(d+j+1,s-j-1);
}


/* d[] の連続する同じ要素を1つにまとめたものをresに格納してresのサイズを返す */
/* 基本的には d はsortedで異なる要素の数を調べる場合とか */
int intUnique(int d[],int size,int res[]){
  int i,n=1;
  if(size==0) return 0;
  res[0]=d[0];
  REP(i,1,size) if(d[i]!=d[i-1]) res[n++] = d[i];
  return n;
}

/* d[i]=sなるa<=i<bを返す．複数ある場合はどれを返すのか不定．該当なしなら-1を返す．d[]はsorted． */
int intArrayBinarySearch(int d[],int s,int a,int b){
  int c = (a+b)/2;
  if(a>=b) return -1;
  if(d[c]==s) return c;
  if(d[c]>s) return intArrayBinarySearch(d,s,a,c);
  return intArrayBinarySearch(d,s,c+1,b);
}

/* dと相対的な大きさを保ったまま，できるだけ小さい数で表した配列をresに返す */
/* d={2,5,3,3,6,5} -> res={0,2,1,1,3,2} */
/* return resの中の最大値+1 (resの中にある数字の種類) */
int intReduction(int d[],int size,int res[],void *WorkMemory){
  int i,j,k,unique_size;
  int *sorted_d,*unique_d;

  sorted_d = (int*) WorkMemory; WorkMemory = (void*) (sorted_d + size);
  unique_d = (int*) WorkMemory;

  rep(i,size) sorted_d[i]=d[i]; intSort(sorted_d,size);
  unique_size = intUnique(sorted_d,size,unique_d);

  rep(i,size) res[i] = intArrayBinarySearch(unique_d,d[i],0,unique_size);
  return unique_size;
}


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

int ori[3000], red[3000];

int xx[3000], yy[3000], xs, ys;
int mp[2200][2200];

int main(){
  int i, j, k, ii, jj;
  int sx, sy, sd, c, nx, ny, nc;
  int dx[4] = {-1,0,1,0}, dy[4] = {0,1,0,-1};
  int T, cnt = 0;
  int res;
  void *mem = malloc(60000000);
  DijkstraHeap<int> heap;

  heap.malloc(5000000*4);
  
  scanf("%d",&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d%d%d",&X,&Y,&N);
    rep(i,N) scanf("%d%d%d%d",xa+i,ya+i,xb+i,yb+i);

    rep(i,X) rep(j,Y) mp[i][j] = 0;
    rep(i,N){
      REP(j,xa[i],xb[i]+1) REP(k,ya[i],yb[i]+1) mp[j][k]++;
    }

    heap.init(X*Y);
    rep(i,Y) heap.change(0*Y+i, mp[0][i]?0:1);
    while(heap.size){
      k = heap.pop();
      i = k/Y; j = k%Y;
      REP(ii,i-1,i+2) REP(jj,j-1,j+2){
        if(ii<0||ii>=X || jj<0||jj>=Y) continue;
        heap.change(ii*Y+jj, heap.val[k]+(mp[ii][jj]?0:1));
      }
    }

    res = 1000000000;
    rep(i,Y) res = min(res, heap.val[(X-1)*Y+i]);
    printf("%d\n",res);
  }

  return 0;
}
