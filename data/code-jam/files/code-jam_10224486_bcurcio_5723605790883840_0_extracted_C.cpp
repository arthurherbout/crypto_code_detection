#include <iostream>
#include <cstdio>
#include <cstring>
#include <map>
#include <algorithm>
#include <cassert>
using namespace std;
int in(){int r=0,c;for(c=getchar_unlocked();c<=32;c=getchar_unlocked());if(c=='-') return -in();for(;c>32;r=(r<<1)+(r<<3)+c-'0',c=getchar_unlocked());return r;}
typedef pair<int,int> pii;

int R,C;
int mat[30][30];

int row(int a,int r,int c){
  return (a / C) + 1;
}

int col(int a, int r, int c){
  return (a % C) + 1;
}

void show(int r, int c){
  
  int i,j;
  for(i=1;i<=R;i++){
    for(j=1;j<=C;j++) cout<<(char)mat[i][j];
    cout << endl;
  }
  
}


int follow(int x,int y,int dir){
  //~ cerr << x << ' ' << y << ' ' << dir << ' ' <<mat[x][y] << endl;
  if(mat[x][y]>99) return mat[x][y] - 100;
  
  //~ cout << x << endl;
  assert(x>0 && x <=R);
  assert(y>0 && y <=C);
  
  if(mat[x][y] == '\\' && dir == 0) return follow(x,y+1,3);
  if(mat[x][y] == '\\' && dir == 1) return follow(x-1,y,2);
  if(mat[x][y] == '\\' && dir == 2) return follow(x,y-1,1);
  if(mat[x][y] == '\\' && dir == 3) return follow(x+1,y,0);
  
  
  if(mat[x][y] == '/' && dir == 0) return follow(x,y-1,1);
  if(mat[x][y] == '/' && dir == 1) return follow(x+1,y,0);
  if(mat[x][y] == '/' && dir == 2) return follow(x,y+1,3);
  if(mat[x][y] == '/' && dir == 3) return follow(x-1,y,2);
  
  return 0;
}


void solve(){
  R = in();
  C = in();
  int N = R*C;
  
  vector<int> pair(2*(R+C)+2);
  
  map<int,pii> inv;
  
  int i;
  for(i=0;i<(R+C);i++){
    int a = in();
    int b = in();
    pair[a] = b;
    pair[b] = a;
  }
  
  for(i=1;i<=C;i++){
    mat[0][i] = i + 100;
    mat[R+1][C + 1 - i] = C + R + i + 100;
    
    inv[i] = pii(1,i);
    inv[C + R + i] = pii(R,C+1-i);
    
  }
  
  for(i=1;i<=R;i++){
    mat[i][C + 1] = C + i + 100;
    mat[R + 1 - i][0] = C + R + C + i + 100 ;
    inv[C+i] = pii(i,C);
    inv[C + R + C + i] = pii(R+1-i,1);
  }
  
  
  
  for(i=0;i<(1<<N);i++){
    
    int j;
    for(j=0;j<N;j++) if(i&(1<<j)){
      mat[row(j,R,C)][col(j,R,C)] = '/';
    }else{
      mat[row(j,R,C)][col(j,R,C)] = '\\';
    }
    
    
    bool can = true;
    
    for(j=1;j<2*(R+C) && can;j++){
      int x,y,dir;
      
      x = inv[j].first;
      y = inv[j].second;
      
      if(j<=C) dir = 0;
      if(j>C && j<=R+C) dir = 1;
      if(j>R+C && j<=R+C+C) dir = 2;
      if(j>R+C+C) dir = 3;
      
      
      if(follow(x,y,dir) != pair[j]) can = false;
    }
    
    if(can){
      show(R,C);
      return;
    }
    
  }
  
  
  cout << "IMPOSSIBLE" << endl;
  
}

int main(){
  for(int i=0,T=in();i<T;i++){
      cout << "Case #"<<i+1 << ": " << endl;
      solve();
  }
}
