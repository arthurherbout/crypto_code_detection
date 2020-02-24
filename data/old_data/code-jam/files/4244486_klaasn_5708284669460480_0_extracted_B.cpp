#include <bits/stdc++.h>

using namespace std;

int getInt() {
  int n;
  cin >> n;
  return n;
}

string keys, target;
int K, L, S;

vector<string> f(int offset) {
  vector<string> s1;
  if(offset == S) {
    s1.push_back("");
  } else {
    auto s2 = f(offset + 1);
    for(int i = 0;i<K;i++) {
      for(auto x : s2) {
	s1.push_back(x + keys[i]);
      }
    }
  }
  return s1;
}



int main() {
  int T = getInt();
  for(int tt=1;tt<=T;tt++) {
    K = getInt();
    L = getInt();
    S = getInt();

    cin >> keys;
    cin >> target;
    vector<string> s = f(0);
    int count = 0, maxocc = 0;
    for(auto x : s) {
      int occ = 0;
      for(int i=0;i<S-L+1;i++) {
	//cout << x.substr(i, L) << " " << target << endl;
	if(x.substr(i, L) == target) occ++;
      }
      maxocc = max(maxocc, occ);
      count += occ;
    }
    //printf("%d %d %d\n", maxocc, count, int(s.size()));
    float ans = 1.0 * maxocc - 1.0 * count / s.size();
    printf("Case #%d: %4.8f\n", tt, ans);
  }
}
