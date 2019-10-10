#include<bits/stdc++.h>
using namespace std;
void rd(int &x){
  int k, m=0;
  x=0;
  for(;;){
    k = getchar_unlocked();
    if(k=='-'){
      m=1;
      break;
    }
    if('0'<=k&&k<='9'){
      x=k-'0';
      break;
    }
  }
  for(;;){
    k = getchar_unlocked();
    if(k<'0'||k>'9'){
      break;
    }
    x=x*10+k-'0';
  }
  if(m){
    x=-x;
  }
}
void wt_L(int x){
  char f[10];
  int m=0, s=0;
  if(x<0){
    m=1;
    x=-x;
  }
  while(x){
    f[s++]=x%10;
    x/=10;
  }
  if(!s){
    f[s++]=0;
  }
  if(m){
    putchar_unlocked('-');
  }
  while(s--){
    putchar_unlocked(f[s]+'0');
  }
}
int main(){
  int TEST, test=0;
  rd(TEST);
  while(TEST--){
    double A[50][50], mn, mx;
    int N, P, Q[50][50], R[50], X, Y, Z, i, j, k, res, st[50];
    printf("Case #%d: ", ++test);
    fprintf(stderr, "r=%d\n", TEST);
    rd(N);
    rd(P);
    {
      int Lj4PdHRW;
      for(Lj4PdHRW=0;Lj4PdHRW<N;Lj4PdHRW++){
        rd(R[Lj4PdHRW]);
      }
    }
    for(i=0;i<N;i++){
      {
        int KL2GvlyY;
        for(KL2GvlyY=0;KL2GvlyY<P;KL2GvlyY++){
          rd(Q[i][KL2GvlyY]);
        }
      }
    }
    for(i=0;i<N;i++){
      sort(Q[i], Q[i]+P);
    }
    mn = 0.9 - 1e-14;
    mx = 1.1 + 1e-14;
    res = 0;
    {
      int Q5VJL1cS;
      for(Q5VJL1cS= 0;Q5VJL1cS< (P-1) + 1;Q5VJL1cS++){
        {
          int e98WHCEY;
          for(e98WHCEY= 0;e98WHCEY< (N-1) + 1;e98WHCEY++){
            A[e98WHCEY][Q5VJL1cS] = (double)Q[e98WHCEY][Q5VJL1cS] / R[e98WHCEY];
          }
        }
      }
    }
    {
      int cTE1_r3A;
      for(cTE1_r3A= 0;cTE1_r3A< (N-1) + 1;cTE1_r3A++){
        st[cTE1_r3A] = 0;
      }
    }
    res = 0;
    k = 1;
    for(;;){
      for(i=0;i<N;i++){
        if(st[i]==P){
          break;
        }
      }
      if(i < N){
        break;
      }
      for(i=0;i<N;i++){
        if(A[i][st[i]] < mn*k){
          break;
        }
      }
      if(i < N){
        st[i]++;
        continue;
      }
      for(i=0;i<N;i++){
        if(A[i][st[i]] > mx*k){
          break;
        }
      }
      if(i < N){
        k++;
        continue;
      }
      res++;
      for(i=0;i<N;i++){
        st[i]++;
      }
    }
    wt_L(res);
    putchar_unlocked('\n');
  }
  return 0;
}
// cLay varsion 20170413-1 [beta]

// --- original code ---
// {
//   int test = 0, TEST;
//   rd(TEST);
//   while(TEST--){
//     printf("Case #%d: ", ++test);
//     fprintf(stderr, "r=%d\n", TEST);
// 
//     int N, P;
//     int R[50], Q[50][50], st[50];
//     double A[50][50], mn, mx;
//     int i, j, k;
//     int X, Y, Z;
//     int res;
// 
//     rd(N,P,R(N));
//     rep(i,N) rd(Q[i](P));
//     rep(i,N) sort(Q[i], Q[i]+P);
// 
//     mn = 0.9 - 1e-14;
//     mx = 1.1 + 1e-14;
// 
//     res = 0;
//     A[0..N-1][0...P-1] = (double)Q[0..][0...] / R[0..];
//     st[0..N-1] = 0;
// 
//     res = 0;
//     k = 1;
//     for(;;){
//       rep(i,N) if(st[i]==P) break;
//       if(i < N) break;
// 
//       rep(i,N) if(A[i][st[i]] < mn*k) break;
//       if(i < N){ st[i]++; continue; }
// 
//       rep(i,N) if(A[i][st[i]] > mx*k) break;
//       if(i < N){ k++; continue; }
// 
//       res++;
//       rep(i,N) st[i]++;
//     }
//     
// 
//     wt(res);
//   }
// }
