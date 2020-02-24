#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

bool cmp(int a, int b) {
  return (__builtin_popcount(a) < __builtin_popcount(b)) ||
    (__builtin_popcount(a) == __builtin_popcount(b) && a < b);
}

int U[10], V[10], nw, nv;

bool same(int bm) {
  for (int i = 0; i < nw; ++i) {
    int u = U[i]-1, v = V[i]-1;
    if (u > v) swap(u, v);
    int a = 0, b = 0;
    for (int j = 0; j < nv; ++j) {
      if (bm & (1<<j)) {
        if (j > u && j < v)
          ++a;
        if (j < u || j > v)
          ++b;
      }
    }

    if (a > 0 && b > 0) return false;
  }
  return true;
}

int main() {
  int T, C = 1;
  cin >> T;
  while (T-- && cin >> nv >> nw) {
    for (int i = 0; i < nw; ++i)
      cin >> U[i];
    for (int i = 0; i < nw; ++i)
      cin >> V[i];

    vector<int> B;
    for (int i = 0; i < (1<<nv); ++i)
      B.push_back(i);
    sort(B.begin(), B.end(), cmp);

    vector<int> R;
    int mn = 1000;
    for (int i = int(B.size()-1); i >= 0; --i) {
      int bm = B[i];
      if (same(bm)) {
        bool f = true;
        for (int i = 0; i < int(R.size()); ++i)
          if ((R[i] & bm) == bm)
            f = false;
        if (f)
          R.push_back(bm), mn = min(mn, __builtin_popcount(bm));
      }
    }
    
    cout << "Case #" << C++ << ": ";
    if (mn > 5) {
      cout << nv << endl;
      for (int i = 0; i < nv; ++i)
        cout << (i ? " " : "") << i+1;
      cout << endl;
    } else {
      
      int best = -1;
      vector<int> B;
      for (int k = mn; k >= 1; --k) {
        int cap = 1;
        for (int j = 0; j < nv; ++j)
          cap *= k;

        for (int j = 0; j < cap; ++j) {
          int cur = j;
          bool f = true;
          
          vector<int> A;
          for (int i = 0; i < nv; ++i)
            A.push_back(cur%k), cur /= k;

          for (int i = 0; f && i < int(R.size()); ++i) {
            set<int> S;
            for (int l = 0; l < nv; ++l)
              if (R[i] & (1<<l))
                S.insert(A[l]);

            if (int(S.size()) < k)
              f = false;
          }
          
          if (f) {
            best = k, B = A;
            break;
          }
        }

        if (best == k)
          break;
      }

      cout << best << endl;
      for (int i = 0; i < int(B.size()); ++i)
        cout << (i ? " " : "") << B[i]+1;

      cout << endl;
    }
  }
}
