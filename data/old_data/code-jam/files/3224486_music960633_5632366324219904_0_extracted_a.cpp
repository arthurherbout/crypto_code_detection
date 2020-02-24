#include <stdio.h>

char g[101][51], b[51];

bool contain_all_one(int n, int l){
   bool flg;
   for(int i=0; i<n; ++i){
      flg = true;
      for(int j=0; j<l; ++j){
         if(g[i][j] == '0')
            flg = false;
      }
      if(flg) return true;
   }
   return false;
}

int main(){
   int T, N, L;
   scanf("%d", &T);
   for(int t=1; t<=T; ++t){
      printf("Case #%d: ", t);
      scanf("%d%d", &N, &L);
      for(int i=0; i<N; ++i){
         scanf("%s", g[i]);
      }
      scanf("%s", b);
      if(contain_all_one(N, L)){
         printf("IMPOSSIBLE\n");
      }
      else if(L == 1){
         printf("0 ?\n");
      }
      else{
         for(int i=0; i<L; ++i)
            printf("0?");
         printf(" ");
         for(int i=0; i<L-1; ++i)
            printf("1");
         printf("\n");
      }
   }

   return 0;
}
