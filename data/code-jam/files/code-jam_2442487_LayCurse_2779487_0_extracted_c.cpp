#include<cstdio>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<utility>
#include<cassert>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

int inA[2100], inB[2100];

int check(int res[], int N){
  int i, j, k, ok;
  int A[2100], B[2100];
  
  A[0] = 1;
  REP(i,1,N){
    A[i] = 1;
    rep(j,i) if(res[i] > res[j]) A[i] = max(A[i], A[j]+1);
  }

  B[N-1] = 1;
  for(i=N-2;i>=0;i--){
    B[i] = 1;
    REP(j,i+1,N) if(res[i] > res[j]) B[i] = max(B[i], B[j]+1);
  }

//  rep(i,N) printf("%d ",A[i]); puts("");
//  rep(i,N) printf("%d ",B[i]); puts("");

  rep(i,N) if(A[i]!=inA[i]) return 0;
  rep(i,N) if(B[i]!=inB[i]) return 0;
  return 1;
  
  rep(i,N) assert(A[i]==inA[i]);
  rep(i,N) assert(B[i]==inB[i]);
}

int main(){
  int i,j,k,l,m,N,cc;
  int res[3000];
  static int edge[3000][3000], deg[3000];
  int st[3000], st_size;
  int size, count = 0;

  scanf("%d",&size);
  while(size--){
    scanf("%d",&N);
    rep(i,N) scanf("%d",inA+i);
    rep(i,N) scanf("%d",inB+i);
    
    rep(i,N) rep(j,N) edge[i][j] = 0;
    rep(i,N) REP(j,i+1,N){
      if(inA[i] >= inA[j]) edge[i][j] = 1;
      if(inB[i] <= inB[j]) edge[j][i] = 1;
    }
    rep(i,N){
      for(j=i-1;j>=0;j--) if(inA[j]==inA[i]-1){
        edge[i][j] = 1;
        break;
      }
    }
    for(i=N-1;i>=0;i--){
      for(j=i+1;j<N;j++) if(inB[j]==inB[i]-1){
        edge[i][j] = 1;
        break;
      }
    }

    rep(i,N) deg[i] = 0;
    rep(i,N) rep(j,N) deg[i] += edge[i][j];

    st_size = 0;
    rep(i,N) if(!deg[i]) st[st_size++] = i;

    cc = 0;
    while(st_size){
      k = st[--st_size];
      res[cc++] = k;
      
      rep(i,N) if(edge[i][k]){
        deg[i]--;
        if(!deg[i]) st[st_size++] = i;
      }
    }
    assert(cc==N);

    rep(i,N) st[i] = res[i];
    rep(i,N) res[st[i]] = i+1;

    assert(check(res,N)==1);
    for(;;){
      int fg = 0;
      rep(i,N) REP(j,i+1,N) if(res[i] > res[j]){
        swap(res[i],res[j]);
        if(check(res,N)==1){
          fg++;
        } else {
          swap(res[i],res[j]);
        }
      }
      if(!fg) break;
    }

    printf("Case #%d:",++count);
    rep(i,N) printf(" %d", res[i]);
    puts("");

    check(res,N);
  }

  return 0;
}
