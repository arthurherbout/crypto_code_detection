#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>

using namespace std;

void check(const size_t& n, int& num_of_unique_digits, vector<int>& f) {
  size_t num = n;
  while (num) {
    int c = num % 10;
    if (f[c] == 0) {
      num_of_unique_digits++;
      f[c] = 1;
    }
    num /= 10;
  }
}

int main() {
  freopen ("A-large.in","r",stdin);
  freopen ("A-large.out","w",stdout);
  size_t T, N;
  cin >> T;
  int t = 1;
  vector<int> f(10, 0);
  while (T--) {
    cin >> N;
    printf("Case #%d: ", t++);

    if (N == 0) {
      printf("INSOMNIA\n");
      continue;
    }

    fill(f.begin(), f.end(), 0);
    int num_of_unique_digits = 0;
    for (size_t i = N;; i += N) {
      check(i, num_of_unique_digits, f);
      if (num_of_unique_digits == 10) {
        cout << i << endl;
        break;
      }
    }
  }
  return 0;
}
