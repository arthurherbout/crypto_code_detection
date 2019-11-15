#include <iostream>
#include <vector>
using namespace std;

// typedef vector<bool> vb;
// typedef vector<vb> vvb;

// int dx[4] = {1, 0, -1, 0};
// int dy[4] = {0, 1, 0, -1};

// int N, M, S;
// vvb visit;

// bool dfs(int x, int y, int m) {
  // if 
  // for (int d = 0; d < 4; ++d)
    // if (dfs(x+dx[d], y+dy[d], m)) return true;
// }

int main() {
  int T; cin >> T;
  for (int t = 1; t <= T; ++t) {
    cout << "Case #" << t << ": ";
    int N, M, K; cin >> N >> M >> K;
    if (N > M) swap(N, M);
    if (N <= 2) cout << K << endl;
    else if (N == 3) {
      if (K < 5) cout << K << endl;
      else if (K < 8 || M < 4) cout << K-1 << endl;
      else if (K < 11 || M < 5) cout << K-2 << endl;
      else if (K < 14 || M < 6) cout << K-3 << endl;
      else cout << K-4 << endl;
    }
    else {
      if (K < 5) cout << K << endl;
      else if (K < 8) cout << K-1 << endl;
      else if (K < 10) cout << K-2 << endl;
      else if (K < 12) cout << K-3 << endl;
      else if (K < 14 || M < 5) cout << K-4 << endl;
      else if (K < 16) cout << K-5 << endl;
      else cout << K-6 << endl;
    }
  }
}
