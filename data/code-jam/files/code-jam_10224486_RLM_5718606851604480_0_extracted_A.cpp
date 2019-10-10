#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
  vector< vector<string> > res(13, vector<string>(3));
  res[0][0] = "P";
  res[0][1] = "R";
  res[0][2] = "S";
  for (int i = 1; i <= 12; ++i) for (int j = 0; j < 3; ++j) {
    string a = res[i-1][j], b = res[i-1][(j+1)%3];
    if (a < b) res[i][j] = a+b;
    else res[i][j] = b+a;
  }
  int T; cin >> T;
  for (int t = 1; t <= T; ++t) {
    cout << "Case #" << t << ": ";
    int N, R, P, S;
    cin >> N >> R >> P >> S;
    int i;
    for (i = 0; i < 3; ++i) {
      const string& s = res[N][i];
      if (count(s.begin(), s.end(), 'R') == R and
          count(s.begin(), s.end(), 'P') == P and
          count(s.begin(), s.end(), 'S') == S) {
        cout << s;
        break;
      }
    } if (i == 3) cout << "IMPOSSIBLE";
    cout << endl;
  }
}