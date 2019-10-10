#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>
using namespace std;

int i, t, T, N, M, ALL;
int f[52][52];
string zip[52], mz;
vector <int> rt;
int cp[52];

bool possible(string route, int from, int to) {
  if (to == N) 
    return true;
  if (f[rt[from]][rt[to]]) {
    if (possible(route + (char)('0'+from), to, to+1)) return true;
  } else {
    while (route.length()) {
      int last = *route.rbegin() - '0';
      route.resize(route.length()-1);
      if (possible(route, last, to)) return true;
    }
  }
  return false;
}

int main(){ 
  cin >> T;
  for (t=1; t<=T; t++) {

    cin >> N >> M;
    memset(f, 0, sizeof(f));
    for (i=1; i<=N; i++) cin >> zip[i];
    for (i=0; i<M; i++) {
      int a, b;
      cin >> a >> b;
      f[a][b] = 1;
      f[b][a] = 1;
    }

    mz = "";
    rt.clear();
    for (i=1; i<=N; i++) rt.push_back(i);

    do {
      if (possible("0", 0, 1)) {
        string z;
        for (i=0;i<N;i++)
          z += zip[rt[i]];
        if (mz.length() == 0 || z.length() < mz.length() || z.length() == mz.length() && z < mz) 
          mz = z;
      }
    } while (next_permutation(rt.begin(), rt.end()));
    
    cout << "Case #" << t << ": " << mz << endl;
  }
}
