#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;

void Input(long long& N, int& P_D, int& P_G) {
  cin >> N >> P_D >> P_G;
}

void Output(int i, bool possible) {
  cout << "Case #" << i+1 << ": ";
  if (possible) {
    cout << "Possible" << endl;
  } else {
    cout << "Broken" << endl;
  }
}

bool Solve(long long N, int P_D, int P_G) {
  if (P_G == 100 || P_G == 0) {
    return P_D == P_G;
  }

  int gcd_P_D_100 = 1;
  
  if (P_D % 25 == 0) {
    gcd_P_D_100 *= 25;
  } else if (P_D % 5 == 0) {
    gcd_P_D_100 *= 5;
  }

  if (P_D % 4 == 0) {
    gcd_P_D_100 *= 4;
  } else if (P_D % 2 == 0) {
    gcd_P_D_100 *= 2;
  }


  return (100 / gcd_P_D_100) <= N;
}

int main() {
  int test_number;
  cin >> test_number;
  
  for (int i = 0; i < test_number; ++i) { 
    long long N;
    int P_D, P_G;
    Input(N, P_D, P_G);
    Output(i, Solve(N, P_D, P_G));
  }
  return 0;
}