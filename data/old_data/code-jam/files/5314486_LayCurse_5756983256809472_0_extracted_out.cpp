#include<bits/stdc++.h>
using namespace std;
void *wmem;
template<class T> void malloc1d(T **arr, int x, void **mem = &wmem){
  (*arr)=(T*)(*mem);
  (*mem)=((*arr)+x);
}
template<class T> void malloc2d(T ***arr, int x, int y, void **mem = &wmem){
  int i;
  (*arr)=(T**)(*mem);
  (*arr)[0]=(T*)((*arr)+x);
  for(i=1;i<x;i++){
    (*arr)[i]=(*arr)[i-1]+y;
  }
  (*mem)=((*arr)[x-1]+y);
}
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
int rd(char c[]){
  int i, sz=0;
  for(;;){
    i = getchar_unlocked();
    if(i!=' '&&i!='\n'&&i!='\r'&&i!='\t'&&i!=EOF){
      break;
    }
  }
  c[sz++] = i;
  for(;;){
    i = getchar_unlocked();
    if(i==' '||i=='\n'||i=='\r'||i=='\t'||i==EOF){
      break;
    }
    c[sz++] = i;
  }
  c[sz]='\0';
  return sz;
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
char memarr[64000000], **mp, **ok;
int S, T, *cost, *dist, dx[4]={-1,1,0,0}, dy[4]={0,0,-1,1}, edge[1024][20][20], reach[1111][1111], res_S[30], res_T[30], ress, tmp_S[30], tmp_T[30], tmps;
void gridGetDistCost01(int X, int Y, int *cost, int sx, int sy, int *res, void *mem, int d=4){
  int XY=X*Y, dx[8]={-1,1,0,0,-1,1,-1,1}, dy[8]={0,0,-1,1,-1,-1,1,1}, i, j, k, nx, ny, *q=(int*)mem, q_ed, q_st;
  q_st = q_ed = X*Y+1;
  q[q_ed++] = sx*Y+sy;
  for(i=0;i<XY;i++){
    res[i] = -1;
  }
  res[sx*Y+sy] = 0;
  while(q_st != q_ed){
    k = q[q_st++];
    sx=k/Y;
    sy=k%Y;
    for(i=0;i<d;i++){
      nx = sx + dx[i];
      ny = sy + dy[i];
      if(nx < 0 || nx >= X || ny < 0 || ny >= Y){
        continue;
      }
      j = nx*Y + ny;
      if(cost[j]==-1){
        continue;
      }
      if(res[j]==-1){
        if(cost[j]==1){
          res[j] = res[k] + 1;
          if(cost[j]==0){
            q[--q_st] = j;
          }
          else{
            q[q_ed++] = j;
          }
        }
        else{
          res[j] = res[k] + 1;
        }
      }
    }
  }
}
void solve2(int mask1, int mask2){
  int i, j, k;
  if(reach[mask1][mask2]){
    return;
  }
  reach[mask1][mask2] = 1;
  if(tmps > ress){
    ress = tmps;
    {
      int Lj4PdHRW;
      for(Lj4PdHRW=0;Lj4PdHRW<(ress-1) + 1;Lj4PdHRW++){
        res_S[Lj4PdHRW] = tmp_S[Lj4PdHRW];
      }
    }
    {
      int KL2GvlyY;
      for(KL2GvlyY=0;KL2GvlyY<(ress-1) + 1;KL2GvlyY++){
        res_T[KL2GvlyY] = tmp_T[KL2GvlyY];
      }
    }
  }
  if(ress==S || ress==T){
    return;
  }
  for(i=0;i<S;i++){
    if(mask1&(1<<i)){
      for(j=0;j<T;j++){
        if(edge[mask2][i][j] && (mask2&(1<<j))){
          tmp_S[tmps] = i;
          tmp_T[tmps] = j;
          tmps++;
          solve2(mask1^(1<<i), mask2^(1<<j));
          tmps--;
        }
      }
    }
  }
}
void solve(){
  int M, Sp[100], Tp[100], X, XY, Y, i, j, k, mask, nx, ny, res, sx, sy;
  rd(Y);
  rd(X);
  rd(M);
  {
    int Q5VJL1cS;
    for(Q5VJL1cS=0;Q5VJL1cS<X;Q5VJL1cS++){
      rd(mp[Q5VJL1cS]);
    }
  }
  XY = X*Y;
  S = T = 0;
  for(i=0;i<X;i++){
    for(j=0;j<Y;j++){
      cost[i*Y+j] = 1;
      if(mp[i][j]=='#'){
        cost[i*Y+j] = -1;
      }
      if(mp[i][j]=='S'){
        Sp[S++] = i*Y+j;
      }
      if(mp[i][j]=='T'){
        Tp[T++] = i*Y+j;
      }
    }
  }
  for(mask=0;mask<1<<T;mask++){
    {
      int e98WHCEY;
      for(e98WHCEY=0;e98WHCEY<(S-1) + 1;e98WHCEY++){
        {
          int cTE1_r3A;
          for(cTE1_r3A=0;cTE1_r3A<(T-1) + 1;cTE1_r3A++){
            edge[mask][e98WHCEY][cTE1_r3A] = 0;
          }
        }
      }
    }
  }
  for(i=0;i<1<<S;i++){
    for(j=0;j<1<<T;j++){
      reach[i][j] = 0;
    }
  }
  for(k=0;k<T;k++){
    {
      int RZTsC2BF;
      for(RZTsC2BF=0;RZTsC2BF<(XY-1) + 1;RZTsC2BF++){
        ok[k][RZTsC2BF] = 0;
      }
    }
    sx = Tp[k] / Y;
    sy = Tp[k] % Y;
    for(j=0;j<4;j++){
      for(i=0;;i++){
        nx = sx + i * dx[j];
        ny = sy + i * dy[j];
        if(nx < 0 || ny < 0 || nx >= X || ny >= Y){
          break;
        }
        if(mp[nx][ny]=='#'){
          break;
        }
        ok[k][nx*Y+ny] = 1;
      }
    }
  }
  for(mask=0;mask<1<<T;mask++){
    for(i=0;i<XY;i++){
      cost[i] = (mp[i/Y][i%Y]=='#')?-1:1;
    }
    for(i=0;i<T;i++){
      if(mask & 1<<i){
        for(j=0;j<XY;j++){
          if(ok[i][j]){
            cost[j] = 2;
          }
        }
      }
    }
    for(i=0;i<T;i++){
      if(mask & 1<<i){
        cost[Tp[i]] = -1;
      }
    }
    for(k=0;k<S;k++){
      gridGetDistCost01(X, Y, cost, Sp[k]/Y, Sp[k]%Y, dist, wmem);
      for(i=0;i<XY;i++){
        if(dist[i] >= 0 && dist[i] <= M){
          for(j=0;j<T;j++){
            if(ok[j][i]){
              edge[mask][k][j] = 1;
            }
          }
        }
      }
    }
  }
  ress = 0;
  tmps = 0;
  solve2((1<<S)-1, (1<<T)-1);
  wt_L(ress);
  putchar_unlocked('\n');
  for(i=0;i<ress;i++){
    wt_L(res_S[i]+1);
    putchar_unlocked(' ');
    wt_L(res_T[i]+1);
    putchar_unlocked('\n');
  }
}
int main(){
  int Case, T;
  wmem = memarr;
  rd(T);
  malloc2d(&mp,200,200);
  malloc2d(&ok,200,50000);
  malloc1d(&cost,50000);
  malloc1d(&dist,50000);
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
// void gridGetDistCost01(int X, int Y, int *cost, int sx, int sy, int *res, void *mem, int d=4){
//   int i, j, k, nx, ny, XY = X*Y;
//   int dx[8] = {-1,1,0,0,-1,1,-1,1}, dy[8] = {0,0,-1,1,-1,-1,1,1};
//   int *q=(int*)mem, q_st, q_ed;
// 
//   q_st = q_ed = X*Y+1;
//   q[q_ed++] = sx*Y+sy;
//   rep(i,XY) res[i] = -1;
//   res[sx*Y+sy] = 0;
//   while(q_st != q_ed){
//     k = q[q_st++];
//     sx=k/Y; sy=k%Y;
//     rep(i,d){
//       nx = sx + dx[i];
//       ny = sy + dy[i];
//       if(nx < 0 || nx >= X || ny < 0 || ny >= Y) continue;
//       j = nx*Y + ny;
//       if(cost[j]==-1) continue;
//       if(res[j]==-1){
//         if(cost[j]==1){
//           res[j] = res[k] + 1;
//           if(cost[j]==0) q[--q_st] = j;
//           else           q[q_ed++] = j;
//         } else {
//           res[j] = res[k] + 1;
//         }
//       }
//     }
//   }
// }
// 
// char **mp, **ok;
// int *cost, *dist;
// int edge[1024][20][20];
// 
// int dx[4] = {-1,1,0,0};
// int dy[4] = {0,0,-1,1};
// 
// int S, T;
// int ress, res_S[30], res_T[30];
// int tmps, tmp_S[30], tmp_T[30];
// 
// int reach[1111][1111];
// 
// void solve2(int mask1, int mask2){
//   int i, j, k;
// 
//   if(reach[mask1][mask2]) return;
//   reach[mask1][mask2] = 1;
// 
//   if(tmps > ress){
//     ress = tmps;
//     res_S[0..ress-1] = tmp_S[0..ress-1];
//     res_T[0..ress-1] = tmp_T[0..ress-1];
//   }
//   if(ress==S || ress==T) return;
// 
//   rep(i,S) if(mask1&(1<<i)){
//     rep(j,T) if(edge[mask2][i][j] && (mask2&(1<<j))){
//       tmp_S[tmps] = i;
//       tmp_T[tmps] = j;
//       tmps++;
//       solve2(mask1^(1<<i), mask2^(1<<j));
//       tmps--;
//     }
//   }
// }
// 
// void solve(){
//   int i, j, k;
//   int X, Y, XY, M;
//   int Sp[100], Tp[100];
// 
//   int mask;
//   int sx, sy, nx, ny;
//   int res;
// 
//   rd(Y,X,M,mp(X));
//   XY = X*Y;
// 
//   S = T = 0;
//   rep(i,X) rep(j,Y){
//     cost[i*Y+j] = 1;
//     if(mp[i][j]=='#') cost[i*Y+j] = -1;
//     if(mp[i][j]=='S') Sp[S++] = i*Y+j;
//     if(mp[i][j]=='T') Tp[T++] = i*Y+j;
//   }
// 
//   rep(mask,1<<T) edge[mask][0...S-1][0..T-1] = 0;
//   rep(i,1<<S) rep(j,1<<T) reach[i][j] = 0;
// 
//   rep(k,T){
//     ok[k][0..XY-1] = 0;
//     sx = Tp[k] / Y;
//     sy = Tp[k] % Y;
//     rep(j,4) for(i=0;;i++){
//       nx = sx + i * dx[j];
//       ny = sy + i * dy[j];
//       if(nx < 0 || ny < 0 || nx >= X || ny >= Y) break;
//       if(mp[nx][ny]=='#') break;
//       ok[k][nx*Y+ny] = 1;
//     }
//   }
// 
//   rep(mask,1<<T){
//     rep(i,XY) cost[i] = (mp[i/Y][i%Y]=='#')?-1:1;
//     rep(i,T) if(mask & 1<<i) rep(j,XY) if(ok[i][j]){
//       cost[j] = 2;
//     }
//     rep(i,T){
//       if(mask & 1<<i) cost[Tp[i]] = -1;
//     }
//     rep(k,S){
//       gridGetDistCost01(X, Y, cost, Sp[k]/Y, Sp[k]%Y, dist, wmem);
//       rep(i,XY) if(dist[i] >= 0 && dist[i] <= M){
//         rep(j,T) if(ok[j][i]) edge[mask][k][j] = 1;
//       }
//     }
//   }
// 
//   ress = 0;
//   tmps = 0;
//   solve2((1<<S)-1, (1<<T)-1);
//   wt(ress);
//   rep(i,ress) wt(res_S[i]+1, res_T[i]+1);
// }
// 
// 
// {
//   int T, Case;
//   rd(T);
// 
//   malloc2d(&mp,200,200);
//   malloc2d(&ok,200,50000);
//   malloc1d(&cost,50000);
//   malloc1d(&dist,50000);
//   
//   rep(Case, T){
//     fprintf(stderr,"%d / %d\n", Case, T);
//     wtF("Case #{Case+1}: ");
//     solve();
//   }
// }
