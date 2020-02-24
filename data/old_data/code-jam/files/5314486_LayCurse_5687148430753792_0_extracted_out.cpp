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
void wt_L(const char c[]){
  int i=0;
  for(i=0;c[i]!='\0';i++){
    putchar_unlocked(c[i]);
  }
}
template<class S, class T> inline S max_L(S a,T b){
  return a>=b?a:b;
}
int B[1000], C, M, N, P[1000], c[1000][1000], cs[1000], narr[1000], ng[1000], oarr[1000], ok[1000];
int solve2(int mx){
  int i, j, k, m;
  {
    int Lj4PdHRW;
    for(Lj4PdHRW=0;Lj4PdHRW<(N-1) + 1;Lj4PdHRW++){
      ok[Lj4PdHRW] = ng[Lj4PdHRW] = 0;
    }
  }
  for(i=0;i<C;i++){
    {
      int KL2GvlyY;
      for(KL2GvlyY=0;KL2GvlyY<(N-1) + 1;KL2GvlyY++){
        oarr[KL2GvlyY] = 0;
      }
    }
    {
      int Q5VJL1cS;
      for(Q5VJL1cS=0;Q5VJL1cS<(N-1) + 1;Q5VJL1cS++){
        narr[Q5VJL1cS] = 0;
      }
    }
    for(j=0;j<cs[i];j++){
      m = c[i][j];
      if(oarr[m]+narr[m] >= mx){
        while(oarr[m]+narr[m] >= mx){
          m--;
        }
        narr[m]++;
      }
      else{
        oarr[m]++;
      }
    }
    for(j=0;j<N;j++){
      ok[j] += oarr[j];
      ng[j] += narr[j];
    }
  }
  m = 0;
  for(i=0;i<N;i++){
    m += ok[i] + ng[i];
    if(m > (i+1)*mx){
      return -1;
    }
    if(ok[i] > mx){
      ng[i] += ok[i] - mx;
      ok[i] = mx;
    }
  }
  {
    int cTE1_r3A, e98WHCEY;
    if(N==0){
      cTE1_r3A = 0;
    }
    else{
      cTE1_r3A = ng[0];
      for(e98WHCEY=1;e98WHCEY<N;e98WHCEY++){
        cTE1_r3A += ng[e98WHCEY];
      }
    }
    return cTE1_r3A;
  }
}
void solve(){
  int i, j, k, md, mn, mx;
  rd(N);
  rd(C);
  rd(M);
  {
    int RZTsC2BF;
    for(RZTsC2BF=0;RZTsC2BF<M;RZTsC2BF++){
      rd(P[RZTsC2BF]);
      rd(B[RZTsC2BF]);
    }
  }
  {
    int FmcKpFmN;
    for(FmcKpFmN=0;FmcKpFmN<(M-1) + 1;FmcKpFmN++){
      P[FmcKpFmN]--;
    }
  }
  {
    int xr20shxY;
    for(xr20shxY=0;xr20shxY<(M-1) + 1;xr20shxY++){
      B[xr20shxY]--;
    }
  }
  {
    int WYIGIcGE;
    for(WYIGIcGE=0;WYIGIcGE<(C-1) + 1;WYIGIcGE++){
      cs[WYIGIcGE] = 0;
    }
  }
  for(i=0;i<M;i++){
    c[B[i]][cs[B[i]]++] = P[i];
  }
  for(i=0;i<C;i++){
    sort(c[i], c[i]+cs[i]);
  }
  {
    int KrdatlYV, t_ynMSdg;
    if(C==0){
      KrdatlYV = 0;
    }
    else{
      KrdatlYV = cs[0];
      for(t_ynMSdg=1;t_ynMSdg<C;t_ynMSdg++){
        KrdatlYV = max_L(KrdatlYV, cs[t_ynMSdg]);
      }
    }
    mn =KrdatlYV;
  }
  mx = M;
  while(mn < mx){
    md = (mn + mx) / 2;
    k = solve2(md);
    if(k == -1){
      mn = md + 1;
    }
    else{
      mx = md;
    }
  }
  k = solve2(mx);
  wt_L(mx);
  putchar_unlocked(' ');
  wt_L(k);
  putchar_unlocked('\n');
}
int main(){
  int Case, T;
  rd(T);
  for(Case=0;Case<T;Case++){
    fprintf(stderr,"%d / %d\n", Case, T);
    wt_L("Case #");
    wt_L(Case+1);
    wt_L(": ");
    solve();
  }
  return 0;
}
// cLay varsion 20170506-1 [beta]

// --- original code ---
// int N, M, C, P[1000], B[1000];
// 
// int c[1000][1000], cs[1000];
// 
// int ok[1000], ng[1000];
// int oarr[1000], narr[1000];
// 
// int solve2(int mx){
//   int i, j, k, m;
// 
//   ok[0..N-1] = ng[0..] = 0;
// 
//   rep(i,C){
//     oarr[0..N-1] = 0;
//     narr[0..N-1] = 0;
//     rep(j,cs[i]){
//       m = c[i][j];
//       if(oarr[m]+narr[m] >= mx){
//         while(oarr[m]+narr[m] >= mx) m--;
//         narr[m]++;
//       } else {
//         oarr[m]++;
//       }
//     }
//     rep(j,N){
//       ok[j] += oarr[j];
//       ng[j] += narr[j];
//     }
//   }
// 
//   m = 0;
//   rep(i,N){
//     m += ok[i] + ng[i];
//     if(m > (i+1)*mx) return -1;
//     if(ok[i] > mx){
//       ng[i] += ok[i] - mx;
//       ok[i] = mx;
//     }
//   }
// 
//   return sum(ng(N));
// }
// 
// void solve(){
//   int i, j, k;
//   int mn, mx, md;
// 
//   rd(N, C, M, (P,B)(M));
// 
//   P[0..M-1]--;
//   B[0..M-1]--;
// 
//   cs[0..C-1] = 0;
//   rep(i,M) c[B[i]][cs[B[i]]++] = P[i];
//   rep(i,C) sort(c[i], c[i]+cs[i]);
// 
//   mn = max(cs(C));
//   mx = M;
//   while(mn < mx){
//     md = (mn + mx) / 2;
//     k = solve2(md);
//     if(k == -1) mn = md + 1; else mx = md;
//   }
//   k = solve2(mx);
//   wt(mx,k);
// }
// 
// {
//   int T, Case;
//   rd(T);
//   rep(Case, T){
//     fprintf(stderr,"%d / %d\n", Case, T);
//     wtF("Case #{Case+1}: ");
//     solve();
//   }
// }
