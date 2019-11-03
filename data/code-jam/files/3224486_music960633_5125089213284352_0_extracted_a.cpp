#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iostream>
#define K 10000

std::vector<int> course[101];
char s[101], ms[5][101];
int sum[101], cnt[5];

void swap(int& x, int& y){
   int t = x;
   x = y;
   y = t;
}

int match(char* s1, char* s2){
   int n1 = strlen(s1);
   int n2 = strlen(s2);
   int i, j;
   for(i=0; i<n1; ++i){
      for(j=0; j<n2 && i+j<n1; ++j){
         if(s1[i+j] != s2[j]) break;
      }
      if(j == n2) return 1;
   }
   return 0;
}

void build(int id){
   if(sum[id] != 0) return;
   sum[id] = 1;
   for(int i=0; i<course[id].size(); ++i){
      build(course[id][i]);
      sum[id] += sum[course[id][i]];
   }
}

void run(int n, int m, int k){
   std::vector<int> vec;
   char str[101];
   for(int i=0; i<=n; ++i)
      sum[i] = 0;
   build(0);
   for(int i=0; i<m; ++i)
      cnt[i] = 0;
   str[n] = '\0';
   while(k--){
      vec = course[0];
      for(int i=0; i<n; ++i){
         int sz = vec.size(), id, course_id;
         float p = 0.0;
         for(int j=0; j<sz; ++j)
            p += 1.0 / sum[vec[j]];
         float r = (float)rand() / RAND_MAX * p;
         float acc = 0.0;
         id = 0;
         for(int j=0; j<sz; ++j){
            if(r < 1.0 / sum[vec[j]] + acc){
               id = j;
               break;
            }
            acc += 1.0 / sum[vec[j]];
         }
         course_id = vec[id];
         swap(vec[id], vec[sz-1]);
         vec.pop_back();
         str[i] = s[course_id-1];
         for(int j=0; j<course[course_id].size(); ++j)
            vec.push_back(course[course_id][j]);
      }
      for(int i=0; i<m; ++i)
         cnt[i] += match(str, ms[i]);
   }
}

int main(){
   int T, N, M;
   int tmp;
   srand(514);
   scanf("%d", &T);
   for(int t=1; t<=T; ++t){
      std::cerr << t << std::endl;
      scanf("%d", &N);
      for(int i=0; i<=N; ++i)
         course[i].clear();
      for(int i=1; i<=N; ++i){
         scanf("%d", &tmp);
         course[tmp].push_back(i);
      }
      scanf("%s", s);
      scanf("%d", &M);
      for(int i=0; i<M; ++i)
         scanf("%s", ms[i]);
      run(N, M, K);
      printf("Case #%d:", t);
      for(int i=0; i<M; ++i)
         printf(" %.2f", (float)cnt[i] / K);
      printf("\n");
   }
   return 0;
}
