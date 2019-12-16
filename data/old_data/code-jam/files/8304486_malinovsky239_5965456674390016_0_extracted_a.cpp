#include <bits/stdc++.h>

using namespace std;

const int N = 105, INF = int(1e9);

int e[N], l[N], d[N], c;
bool used[N];

int dfs(int pos, int h, int state) {
  if (state == c * 2 && pos == 0) {
    return h;    
  }    
  int res[2];
  res[0] = res[1] = INF;
  for (int edge = pos * 2; edge < pos * 2 + 2; edge++) {
    if (!used[edge]) {
      int dep = h / 24 * 24 + l[edge];
      if (dep < h) {
        dep += 24;  
      }
      //cerr << pos << " " << h << " " << dep << endl; 
      used[edge] = 1;
      res[edge - pos * 2] = dfs(e[edge], dep + d[edge], state + 1);
      used[edge] = 0;
    }
  }
  return min(res[0], res[1]);  
}

int main() {
  int t;
  cin >> t;
  for (int i = 0; i < t; i++) {
    cin >> c;
    for (int j = 0; j < 2 * c; j++) {
      used[j] = false;
      cin >> e[j] >> l[j] >> d[j];
      e[j]--;
    }
    cout << "Case #" << i + 1 << ": " << dfs(0, 0, 0) << endl;
  }
  return 0;  
}
