#include <iostream>
using namespace std;

long double C[50][50][50], choose[50][50];

long double ex(int n, int c, int k) {
  long double &N = C[n][c][k];
  if (!(N < 0)) return N;
  if (!k) return 0;

  long double m = choose[n][c];
  long double pr = 0;
  if (n-k >= c) pr = choose[n-k][c];

  long double ans = 0;
  for (int i = 1; i <= min(c, k); ++i) {
    if (n-k >= c-i)
      ans += choose[n-k][c-i]/m*choose[k][i]*(1 + ex(n, c, k-i));
  }

  N = (pr + ans*m)/(m - pr);

  return N;
}


int main() {
  choose[0][0] = 1;
  for (int i = 0; i <= 40; ++i)
    for (int j = 0; j <= i; ++j)
      if (!j || j == i)
	choose[i][j] = 1;
      else
	choose[i][j] = choose[i-1][j-1] + choose[i-1][j];

  for (int i = 0; i < 50; ++i)
    for (int j = 0; j < 50; ++j)
      for (int k = 0; k < 50; ++k)
	C[i][j][k] = -1;


  int nc;
  cin >> nc;

  for (int i = 1; i <= nc; ++i) {
    int n, c;
    cin >> n >> c;
    cout.setf(ios::fixed);
    cout.precision(7);
    cout << "Case #" << i << ": " << 1 + ex(n, c, n-c) << endl;
  }
}
