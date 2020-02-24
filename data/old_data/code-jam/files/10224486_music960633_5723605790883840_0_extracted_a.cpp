#include <stdio.h>

int p[101], par[101];

void init(){
   for(int i=0; i<101; ++i) par[i] = i;
}
int root(int x){
   if(par[x] == x) return x;
   return par[x] = root(par[x]);
}
bool same(int x, int y){
   return root(x) == root(y);
}
void merge(int x, int y){
   int rx = root(x);
   int ry = root(y);
   if(rx != ry)
      par[rx] = ry;
}

int pos2index(int i, int j, int dir, int N, int M){
   if(dir == 0) return i     * (2*M+1) + j;
   if(dir == 1) return i     * (2*M+1) + M + j + 1;
   if(dir == 2) return (i+1) * (2*M+1) + j;
   if(dir == 3) return i     * (2*M+1) + M + j;
}

int edge2index(int e, int N, int M){
   if(e < M) return pos2index(0, e, 0, N, M);
   if(e < N+M) return pos2index(e-M, M-1, 1, N, M);
   if(e < N+2*M) return pos2index(N-1, N+2*M-1-e, 2, N, M);
   if(e < 2*N+2*M) return pos2index(2*N+2*M-1-e, 0, 3, N, M);
}

bool check(unsigned s, int N, int M){
   init();
   for(int i=0; i<N; ++i){
      for(int j=0; j<M; ++j){
         int idx = i*M + j;
         if((s>>idx) & 1){ // '/'
            merge(pos2index(i, j, 0, N, M), pos2index(i, j, 3, N, M));
            merge(pos2index(i, j, 1, N, M), pos2index(i, j, 2, N, M));
         }
         else{ // '\\'
            merge(pos2index(i, j, 0, N, M), pos2index(i, j, 1, N, M));
            merge(pos2index(i, j, 2, N, M), pos2index(i, j, 3, N, M));
         }
      }
   }
   for(int i=0; i<2*(N+M); i+=2){
      if(!same(edge2index(p[i]-1, N, M), edge2index(p[i+1]-1, N, M)))
         return false;
   }
   return true;
}

void report(unsigned s, int N, int M){
   for(int i=0; i<N; ++i){
      for(int j=0; j<M; ++j){
         int idx = i*M + j;
         printf("%c", (s>>idx) & 1 ? '/' : '\\');
      }
      printf("\n");
   }
}

int main(){
   int T, N, M;
   scanf("%d", &T);
   for(int t=1; t<=T; ++t){
      printf("Case #%d:\n", t);
      scanf("%d%d", &N, &M);
      for(int i=0; i<2*(N+M); ++i) 
         scanf("%d", &p[i]);
      bool flg = false;
      for(unsigned s=0; s<(1<<(N*M)); ++s){
         if(check(s, N, M)){
            report(s, N, M);
            flg = true;
            break;
         }
      }
      if(!flg)
         printf("IMPOSSIBLE\n");
   }
   return 0;
}
