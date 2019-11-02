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
template<class S, class T> inline S chmax(S &a, T b){
  if(a<b){
    a=b;
  }
  return a;
}
int G[100], N, P, arr[4], ok2[1][1]={
  {2}
}, ok3[3][2]={
  {3,0},
  {0,3},
  {1,1}
}, ok4[5][3]={
  {4,0,0},
  {0,2,0},
  {0,0,4},
  {0,1,2},
  {2,1,0}
};
int solve2(int dep, int arr[]){
  int i, j, ng, res=0, tmp;
  if(dep==1){
    return res;
  }
  for(i=0;;i++){
    ng = 0;
    for(j=0;j<1;j++){
      arr[j] -= ok2[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng==0){
      tmp = solve2(dep+1, arr) + i;
      chmax(res, tmp);
    }
    for(j=0;j<1;j++){
      arr[j] += ok2[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng){
      break;
    }
  }
  return res;
}
int solve3(int dep, int arr[]){
  int i, j, ng, res=0, tmp;
  if(dep==3){
    return res;
  }
  for(i=0;;i++){
    ng = 0;
    for(j=0;j<2;j++){
      arr[j] -= ok3[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng==0){
      tmp = solve3(dep+1, arr) + i;
      chmax(res, tmp);
    }
    for(j=0;j<2;j++){
      arr[j] += ok3[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng){
      break;
    }
  }
  return res;
}
int solve4(int dep, int arr[]){
  int i, j, ng, res=0, tmp;
  if(dep==5){
    return res;
  }
  for(i=0;;i++){
    ng = 0;
    for(j=0;j<3;j++){
      arr[j] -= ok4[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng==0){
      tmp = solve4(dep+1, arr) + i;
      chmax(res, tmp);
    }
    for(j=0;j<3;j++){
      arr[j] += ok4[dep][j] * i;
      if(arr[j] < 0){
        ng = 1;
      }
    }
    if(ng){
      break;
    }
  }
  return res;
}
void solve(){
  int cTE1_r3A, e98WHCEY, i, j, k, res, tot;
  rd(N);
  rd(P);
  {
    int Lj4PdHRW;
    for(Lj4PdHRW=0;Lj4PdHRW<N;Lj4PdHRW++){
      rd(G[Lj4PdHRW]);
    }
  }
  {
    int KL2GvlyY;
    for(KL2GvlyY=0;KL2GvlyY<(3) + 1;KL2GvlyY++){
      arr[KL2GvlyY] = 0;
    }
  }
  {
    int Q5VJL1cS;
    for(Q5VJL1cS=0;Q5VJL1cS<(N-1) + 1;Q5VJL1cS++){
      arr[G[Q5VJL1cS]%P]++;
    }
  }
  res = arr[0];
  if(P==2){
    res += solve2(0, arr+1);
  }
  if(P==3){
    res += solve3(0, arr+1);
  }
  if(P==4){
    res += solve4(0, arr+1);
  }
  if(N==0){
    cTE1_r3A = 0;
  }
  else{
    cTE1_r3A = G[0];
    for(e98WHCEY=1;e98WHCEY<N;e98WHCEY++){
      cTE1_r3A += G[e98WHCEY];
    }
  }
  if(cTE1_r3A%P!=0){
    res++;
  }
  wt_L(res);
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
// int N, P, G[100];
// int arr[4];
// 
// int ok2[1][1] = {
//   {2}
// };
// int ok3[3][2] = {
//   {3,0},
//   {0,3},
//   {1,1}
// };
// int ok4[5][3] = {
//   {4,0,0},
//   {0,2,0},
//   {0,0,4},
//   {0,1,2},
//   {2,1,0}
// };
// 
// int solve2(int dep, int arr[]){
//   int i, j, ng, res = 0, tmp;
// 
//   if(dep==1) return res;
// 
//   for(i=0;;i++){
//     ng = 0;
//     rep(j,1){
//       arr[j] -= ok2[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng==0){
//       tmp = solve2(dep+1, arr) + i;
//       res >?= tmp;
//     }
//     rep(j,1){
//       arr[j] += ok2[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng) break;
//   }
// 
//   return res;
// }
// 
// int solve3(int dep, int arr[]){
//   int i, j, ng, res = 0, tmp;
// 
//   if(dep==3) return res;
// 
//   for(i=0;;i++){
//     ng = 0;
//     rep(j,2){
//       arr[j] -= ok3[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng==0){
//       tmp = solve3(dep+1, arr) + i;
//       res >?= tmp;
//     }
//     rep(j,2){
//       arr[j] += ok3[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng) break;
//   }
// 
//   return res;
// }
// 
// int solve4(int dep, int arr[]){
//   int i, j, ng, res = 0, tmp;
// 
//   if(dep==5) return res;
// 
//   for(i=0;;i++){
//     ng = 0;
//     rep(j,3){
//       arr[j] -= ok4[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng==0){
//       tmp = solve4(dep+1, arr) + i;
//       res >?= tmp;
//     }
//     rep(j,3){
//       arr[j] += ok4[dep][j] * i;
//       if(arr[j] < 0) ng = 1;
//     }
//     if(ng) break;
//   }
// 
//   return res;
// }
// 
// void solve(){
//   int i, j, k, tot;
//   int res;
// 
//   rd(N,P,G(N));
//   arr[0..3] = 0;
//   arr[G[0..N-1]%P]++;
// 
//   res = arr[0];
//   if(P==2) res += solve2(0, arr+1);
//   if(P==3) res += solve3(0, arr+1);
//   if(P==4) res += solve4(0, arr+1);
// 
//   if(sum(G(N))%P!=0) res++;
// 
//   wt(res);
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
