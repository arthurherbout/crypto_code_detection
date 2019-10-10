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

void *mem = malloc(100000);

int T, N, KK;
char mode[1200]; int ID[1200], V[1200];
int res;

int memo[17][1000000];

int brute(int depth, int mask){
  int i, j, k;
  int res = 10000, tmp;

  if(memo[depth][mask] >= 0) return memo[depth][mask];

  if(depth==N){
    res = 0;
    rep(i,N) if(mask & 1<<i) res++;
    return memo[depth][mask] = res;
  }

  if(ID[depth]==0){
    rep(i,N){
      if( (mask & 1<<i) && mode[depth]=='E') continue;
      if(!(mask & 1<<i) && mode[depth]=='L') continue;
      tmp = brute(depth+1, mask^(1<<i));
      res = min(res, tmp);
    }
  } else {
    if( (mask & 1<<ID[depth]) && mode[depth]=='E') return 10000;
    if(!(mask & 1<<ID[depth]) && mode[depth]=='L') return 10000;
    return memo[depth][mask] = brute(depth+1, mask^(1<<ID[depth]));
  }

  return memo[depth][mask] = res;
}

int main(){
  int i, j, k;
  int cnt = 0;

  scanf("%d",&T);
  while(T--){
    fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);
    
    scanf("%d",&N);
    rep(i,N) scanf("%s%d",mode+i,ID+i);

    ID[N] = 0;
    KK = intReduction(ID, N+1, V, mem);
    rep(i,N+1) ID[i] = V[i];

    rep(i,N+1) rep(j,1<<N) memo[i][j] = -1;

    res = 10000;
    rep(k,1<<N){
      i = brute(0, k);
      if(i==10000) continue;
      res = min(i,res);
    }

    if(res==10000) puts("CRIME TIME"); else printf("%d\n",res);
  }

  return 0;
}
