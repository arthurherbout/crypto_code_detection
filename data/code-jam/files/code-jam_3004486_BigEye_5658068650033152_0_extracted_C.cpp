#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int tn, n, m, K;

int main(int argc, char *argv[])
{
  cin >> tn;
  for (int ti = 1; ti <= tn; ti ++ ) {
    cin >> n >> m >> K;
    int mins = K;
    for (int i = 1; i <= n-2; ++i) {
      for (int j = 1; j <= m-2; ++j) {
        int cover = 2 * (i + j) + i * j;
        int ans = 2 * (i + j);
        if (cover < K)
          ans += K - cover;
        mins = min(mins, ans);
      }
    }
    for (int i = 1; i <= n-2; i += 2) {
      int ans = (i / 2 + 1) * 4;
      int fill = i + (i / 2) * (1 + i-2);
      if (i == 1) fill = 1;
      int cover = ans + fill;
      // cout << i << " " <<  ans << " " << fill << endl;
      if (cover < K)
        ans += K - cover;
      mins = min(mins, ans);
    }
    cout << "Case #" << ti << ": " << mins << endl;
  }
}
    
