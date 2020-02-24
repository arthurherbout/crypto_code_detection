#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int n, c[80];
bool graph[80][80];

int singlePlayer(int u, int p) {
  int best = 0;
  for (int i=0; i<n; ++i)
    if (i!=p && graph[u][i])
      best = max(best, c[i] + singlePlayer(i, u));
  return best;
}

int recurse(int me, int him) {
  int best = -1e9;
  for (int i=0; i<n; ++i)
    if (graph[me][i]) {
      graph[me][i] = false;
      graph[i][me] = false;
      int oldC = c[i];
      c[i] = 0;
      best = max(best, oldC - recurse(him, i));
      c[i] = oldC;
      graph[me][i] = true;
      graph[i][me] = true;
    }
  if (best == -1e9)
    best = -singlePlayer(him, -1);
  return best;
}

int main() {
  int Z;
  cin >> Z;
  for (int z=1;z<=Z;++z) {
    cin >> n;
    for (int i=0;i<n;++i) cin >> c[i];
    for (int i=0;i<n;++i)
      for (int j=0;j<n;++j)
        graph[i][j]=false;
    for (int i=0;i<n-1;++i) {
      int j;
      cin >> j;
      --j;
      graph[i][j] = true;
      graph[j][i] = true;
    }
    int best = -1e9;
    for (int me=0; me<n; ++me) {
      int oldMeC = c[me];
      c[me] = 0;
      int bestHim = -1e9;
      for (int him=0; him<n; ++him) {
        int oldHimC = c[him];
        c[him] = 0;
        bestHim = max(bestHim, oldHimC - recurse(me, him));
        c[him] = oldHimC;
      }
      best = max(best, oldMeC - bestHim);
      c[me] = oldMeC;
    }
    cout << "Case #" << z << ": " << best << endl;
  }
}

