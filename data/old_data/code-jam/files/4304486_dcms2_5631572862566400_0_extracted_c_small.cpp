#include <bits/stdc++.h>

using namespace std;

int bff_id[15], num_bits[1 << 15];

bool my_cmp(int a, int b) {
  return num_bits[a] > num_bits[b];
}

int main() {
  num_bits[0] = 0;
  for (int i = 1, fim = 1 << 11; i < fim; ++i) {
    num_bits[i] = 1 + num_bits[i - (i & -i)];
  }

  int nt; scanf("%d", &nt);
  for (int caso = 1; caso <= nt; ++caso) {
    int n; scanf("%d", &n);
    for (int i = 0; i < n; ++i) scanf("%d", &bff_id[i]), --bff_id[i];
  
    vector<int> masks;
    for (int i = 0, fim = 1 << n; i < fim; ++i) {
      masks.push_back(i);
    }

    sort(masks.begin(), masks.end(), my_cmp);

    for (int mask_idx = 0; mask_idx < masks.size(); ++mask_idx) {
      int mask = masks[mask_idx];
      vector<int> vec;
      for (int i = 0; i < n; ++i) {
        if (mask & (1 << i)) {
          vec.push_back(i);
        }
      }

      do {
        bool ok = true;
        for (int i = 0; i < vec.size(); ++i) {
          int nxt = (i + 1) % vec.size();
          int prv = (i > 0) ? (i - 1) : (vec.size() - 1);
          if (bff_id[vec[i]] != vec[nxt] &&
              bff_id[vec[i]] != vec[prv]) {
            ok = false;
          }
        }
        
        if (ok) {
          printf("Case #%d: %d\n", caso, (int) vec.size());
          goto nxt;
        }
      } while (next_permutation(vec.begin(), vec.end()));
    }
    nxt:;
  }
  return 0;
}
