#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define REP(i, n) FOR(i, 0, n)
#define TRACE(x) cout << #x << " = " << x << endl
#define _ << " _ " <<

typedef long long llint;

const int MAX = 110;

int f[1<<10][1<<10];
pair<int, int> r[1<<10][1<<10];

char a[MAX][MAX];
int no[MAX][MAX];
int bio[MAX][MAX];
int cookie;
int C, R, M;

int dfs(int x, int y, int mt, int rem_steps) {
  if (x < 0 || x >= R || y < 0 || y >= C) return 0;
  if (bio[x][y] == cookie || a[x][y] == '#') return 0;
  
  bio[x][y] = cookie;
  
  int ret = no[x][y];
  if (rem_steps == 0) return ret;

  if ((no[x][y] & mt) == 0) {
    ret |= dfs(x+1, y, mt, rem_steps-1);
    ret |= dfs(x-1, y, mt, rem_steps-1);
    ret |= dfs(x, y+1, mt, rem_steps-1);
    ret |= dfs(x, y-1, mt, rem_steps-1);
  }
  return ret;
}

int main(void) {
  int TC;
  scanf("%d", &TC);
  for (int tp = 1; tp <= TC; ++tp, fflush(stdout)) {
    scanf("%d %d %d", &C, &R, &M);
    REP(i, R) REP(j, C) no[i][j] = 0;

    vector<pair<int, int>> s, t;
    REP(i, R) scanf("%s", a[i]);
    REP(j, C) REP(i, R) {
      if (a[i][j] == 'S') {
        s.push_back({i, j});
      } else if (a[i][j] == 'T') {
        int idx = t.size();
        for (int x = i; x < R && a[x][j] != '#'; ++x) no[x][j] |= 1<<idx;
        for (int x = i; x >= 0 && a[x][j] != '#'; --x) no[x][j] |= 1<<idx;
        for (int y = j; y < C && a[i][y] != '#'; ++y) no[i][y] |= 1<<idx;
        for (int y = j; y >= 0 && a[i][y] != '#'; --y) no[i][y] |= 1<<idx;
        t.push_back({i, j});
      }
    }
    
    int S = s.size();
    int T = t.size();
    REP(mt, 1<<T) {
      vector<int> canKill(S, 0);
      REP(i, S) {
        cookie++;
        canKill[i] = dfs(s[i].first, s[i].second, mt, M);
      }
      
      REP(ms, 1<<S) {
        f[ms][mt] = 0;
        REP(i, S) REP(j, T) {
          if ((ms & (1<<i)) && (mt & (1<<j)) && (canKill[i] & (1<<j))) {
            int nf = f[ms ^ (1<<i)][mt ^ (1<<j)] + 1;
            if (nf > f[ms][mt]) {
              f[ms][mt] = nf;
              r[ms][mt] = {i, j};
            }
          }
        }
      }
    }


    printf("Case #%d: ", tp);
    
    int ms = (1<<S) - 1;
    int mt = (1<<T) - 1;
    int best = f[ms][mt];
    printf("%d\n", best);
    REP(i, best) {
      int x = r[ms][mt].first;
      int y = r[ms][mt].second;
      printf("%d %d\n", x+1, y+1);
      ms ^= 1<<x;
      mt ^= 1<<y;
    }
  }
  return 0;
}
