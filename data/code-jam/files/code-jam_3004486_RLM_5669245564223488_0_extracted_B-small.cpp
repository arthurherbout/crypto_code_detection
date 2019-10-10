#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<bool> vb;
typedef vector<int> vi;
typedef vector<string> vs;

int factorial[11] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};

void simplify(string &s) {
  string n = "";
  n += s[0];
  for (int i = 1; i < (int)s.size(); ++i)
    if (s[i] != s[i-1]) n += s[i];
  s = n;
}

int check(const string& s) {
  vb seen('z'-'a'+1, false);
  int i = 0;
  while (i < (int)s.size()) {
    if (seen[s[i]-'a']) return 0;
    else seen[s[i]-'a'] = true;
    ++i;
    while (i < (int)s.size() && s[i] == s[i-1]) ++i;
  }
  return 1;
}

int main() {
  int T; cin >> T;
  for (int t = 1; t <= T; ++t) {
    cout << "Case #" << t << ": ";
    int N; cin >> N;
    int c = 0;
    vs w(N);
    for (int i = 0; i < N; ++i) {
      cin >> w[i];
      simplify(w[i]);
    }
    vi p(N); for (int i = 0; i < N; ++i) p[i] = i;
    for (int i = 0; i < factorial[N]; ++i) {
      string r = "";
      for (int j = 0; j < N; ++j) r += w[p[j]];
      // simplify(r);
      c += check(r);
      next_permutation(p.begin(), p.end());
    }
    cout << c << endl;
  }
}
