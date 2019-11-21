#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

int p;
long long n;
long long e[10000], f[10000];
long long decompressed[(1<<20)+10];

int main() {
  int cases;
  cin.sync_with_stdio(false);
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    cin >> p;
    long long tot = 0;
    for(int i = 0; i < p; ++i) 
      cin >> e[i];
    for(int i = 0; i < p; ++i) {
      cin >> f[i];
      tot += f[i];
    }
    n = 1;
    while((1<<n) < tot) ++n;

    int d = 0;
    for(int i = 0; i < p; ++i) 
      for(int j = 0; j < f[i]; ++j)
        decompressed[d++] = e[i];
    sort(decompressed, decompressed + (1<<n));
    assert(decompressed[0] == 0);

    map<int, int> taken_care_of;
    vector<int> ret;
    vector<int> powers(1, 0);
    powers.reserve(1 << n);
    for(int i = 1; i < (1<<n); ++i) {
      int d = decompressed[i];
      auto it = taken_care_of.find(d);
      if(it != taken_care_of.end() && it->second > 0) {
        it->second--;
      }
      else {
        ret.push_back(d);
        // printf("adding %d to sol\n", d);
        int t = powers.size();
        for(int i = 0; i < t; ++i) {
          powers.push_back(powers[i] + d);
          // printf("new power element %d\n", powers[i] + d);
          if(i > 0) 
            taken_care_of[powers[i] + d]++;
        }
      }
      if(ret.size() == n) break;
    }
    printf("Case #%d:", cc+1);
    for(int i = 0; i < ret.size(); ++i)
      printf(" %d", ret[i]);
    printf("\n");
  }
}

