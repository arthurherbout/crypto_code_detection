#include <bitset>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

char test[33] = {};
long divisors[11] = {};

long non_elem_divisor(long long v) {
  if ((v % 2) == 0) {
    return 2;
  } else {
    for (int d = 3; d < 1000; ++d) {
      if ((v % d) == 0) {
        return d;
      }
//      cout << '.';
    }
    return -1;
  }
}

int main(int argc, char **argv) {
  string line;
  string::size_type sz;
  int N, J;
  getline(cin, line);
  getline(cin, line);
  char *end;
  const char* p = line.c_str();
  N = strtol(p, &end, 10);
  p = end;
  J = strtol(p, &end, 10);
  test[0] = '1';
  test[N-1] = '1';
  long tested = 0;

// cout << N << ' ' << J << endl;
  cout << "Case #1:" << endl;
  while (J > 0) {
    bitset< 30 > incr(tested);
    for (int i = 0, ctr = N-2; ctr > 0; ++i, --ctr) {
      if (incr.test(i)) {
        test[ctr] = '1';
      } else {
        test[ctr] = '0';
      }
// cout << '*' << i << '.' << ctr << ';' << test[ctr];
    }
// cout << endl;
    test[N] = '\0';
// cout << test << endl;
// cout << incr << endl;
    for (int base = 2; base <= 10; ++base) {
      long long val = strtoll(test, NULL, base);
// cout << 'b' << base << '=' << val << endl;
      long divisor = non_elem_divisor(val);
      if (-1 == divisor) {
        goto contin;   
      } else {
        divisors[base] = divisor;
      }
    }
    cout << strtol(test, NULL, 10);
    for (int base = 2; base <= 10; ++base) {
      cout << ' ' << divisors[base];
    }
    cout << endl;
    --J;
    contin:
    tested += 1;
// cout << '*' << tested << endl;
  }
  return 0;
}

