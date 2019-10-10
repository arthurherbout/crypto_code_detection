#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <list>
#include <cstring>
#include <stack>
#include <bitset>

using namespace std;

#define NMAX 2147483647
#define LMAX 9223372036854775807LL
#define pb push_back
#define pob pop_back
#define mp make_pair
#define st first
#define nd second
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i,a,b) for(int i=(a);i>(b);--i)
#define REP(i,n) FOR(i,0,n)
#define FORE(i,a,b) for(int i=(a);i<=(b);++i)

char S[55][55];
int num[55][55];
int hit;
bool visit[55][55];

void dfs(int x, int y, int r, int c) {
  hit++;
  visit[x][y] = 1;
  int dx[] = {-1, -1, -1,  0, 0,  1, 1, 1};
  int dy[] = {-1,  0,  1, -1, 1, -1, 0, 1};
  REP(k, 8) {
    int tx = x + dx[k];
    int ty = y + dy[k];
    if(0 <= tx && tx < r && 0 <= ty && ty < c && num[tx][ty] == 0 && !visit[tx][ty])
      dfs(tx, ty, r, c);
  }
}

bool possible(int r, int c, int cnt) {
  if(r * c - cnt == 1)
    return 1;
  int dx[] = {-1, -1, -1,  0, 0,  1, 1, 1};
  int dy[] = {-1,  0,  1, -1, 1, -1, 0, 1};
  memset(num,0,sizeof(num));
  REP(i,r)REP(j,c) {
    if(S[i][j] == '*') {
      num[i][j] = -1;
      continue;
    }
    REP(k,8) {
      int x = i + dx[k];
      int y = j + dy[k];
      if(0 <= x && x < r && 0 <= y && y < c && S[x][y] == '*')
        num[i][j]++;
    }
  }
  int target = 0;
  REP(i,r)REP(j,c) {
    if(num[i][j] <= 0) {
      if(num[i][j] == 0) target++;
      continue;
    }
    int p = 0;
    REP(k,8) {
      int x = i + dx[k];
      int y = j + dy[k];
      if(0 <= x && x < r && 0 <= y && y < c && num[x][y] == 0)
        p++;
    }
    if(!p) return 0;
  }

  REP(i,r)REP(j,c) {
    if(num[i][j] == 0) {
      hit = 0;
      memset(visit,0,sizeof(visit));
      dfs(i, j, r, c);
      return (hit == target);
    }
  }
  return 1;
}

void solve(int r, int c, int k) {
  /*int mines = r * c - k;
  if(r == 1 || c == 1 || k == 1 || !mines) {
    REP(i,r){
      REP(j,c)
        if(!i && !j) printf("c"), k--;
        else if(k) printf("."), k--;
        else printf("*");
      printf("\n");
    }
    return;
  }
  if(k == 2 || k == 3 || k == 5 || k == 7) {
    printf("%d %d %d ",r,c,k);
    printf("Impossible\n");
    return;
  }
  if(r == 2 || c == 2) {
    if(k % 2) {
      printf("%d %d %d ",r,c,k);
      printf("Impossible\n");
    }
    else {
      REP(i,r){
        REP(j,c)
          if(!i && !j) printf("c"), k--;
          else if(k) printf("."), k--;
          else printf("*");
        printf("\n");
      }
    }
    return;
  }*/
  printf("%d %d %d\n",r,c,k);
  FORE(lippin,1,r) {
    int temp = k/lippin + !!(k % lippin);
    if(temp > c)
      continue;
    REP(i,r)REP(j,c)S[i][j] = '*';
    int row = 0, col = 0;
    for(int g6 = 0; g6 < k; g6++) {
      S[row++][col] = '.';
      if(row % lippin == 0) {
        col++;
        row = 0;
      }
    }
    if(k % lippin == 1 && col) {
      swap(S[0][0], S[row][col]);
    }
    if(possible(r,c,r*c-k)) {
      bool ok = 0;
      REP(i,r) {
        REP(j,c) {
          if(!ok && num[i][j] == 0)
            cout << "c", ok = 1;
          else
            cout << S[i][j];
        }
        cout << endl;
      }
      return;
    }
  }
  FORE(lippin,1,c) {
    int temp = k/lippin + !!(k % lippin);
    if(temp > r)
      continue;
    REP(i,r)REP(j,c)S[i][j] = '*';
    int row = 0, col = 0;
    for(int g6 = 0; g6 < k; g6++) {
      S[row][col++] = '.';
      if(col % lippin == 0) {
        row++;
        col = 0;
      }
    }
    if(k % lippin == 1 && row) {
      swap(S[0][0], S[row][col]);
    }
    if(possible(r,c,r*c-k)){
      bool ok = 0;
      REP(i,r) {
        REP(j,c) {
          if(!ok && num[i][j] == 0)
            cout << "c", ok = 1;
          else
            cout << S[i][j];
        }
        cout << endl;
      }
      return;
    }
  }
  cout << "Impossible\n";
}

int main() {
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int TS;
  cin >> TS;
  FORE(ts,1,TS) {
    int r, c, k;
    cin >> r >> c >> k;
    cout << "Case #" << ts << ":" << endl;
    solve(r, c, r*c-k);
  }
  return 0;
}
