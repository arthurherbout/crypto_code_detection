#include <cstdio>
#include <algorithm>
#include <string>
using namespace std;

char buf[100];
int main() {
  int T; scanf("%d", &T);
  for (int it = 1; it <= T; it++) {
    scanf("%s", buf);
    string s(buf);
    bool result = next_permutation(s.begin(), s.end());
    if (result)
      printf("Case #%d: %s\n", it, s.c_str());
    else {
      sort(s.begin(), s.end());
      int nz = 1;
      while (s[0] == '0') {s=s.substr(1); nz++;}
      printf("Case #%d: %c", it, s[0]);
      for (int i = 0; i < nz; i++) printf("0");
      printf("%s\n", s.substr(1).c_str());
    }
  }
}
