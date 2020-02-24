#include <iostream>
using namespace std;

int main() {
  int T; cin >> T;
  for (int test = 1; test <= T; ++test) {
    int R, C, W; cin >> R >> C >> W;
    cout << "Case #" << test << ": " << (C-1)/W + W << endl;
  }
}
