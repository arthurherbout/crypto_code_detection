#include<cstdio>
#include<cassert>
#include<cstring>
#include<map>
#include<set>
#include<time.h>
#include<algorithm>
#include<stack>
#include<queue>
#include<utility>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

void print(const vector<int>& r) {
  for(auto x: r) printf("%d ", x);
  printf("\n");
}

vector<int> getMaxEnclosure(int h, int w) {

  if(w <= 2) {
    vector<int> ret(w * h + 1);
    for(int height = 1; height <= h; ++height) {
      ret[height * w] = height * w;
    }
    for(int i = 1; i <= w * h; ++i) {
      ret[i] = max(ret[i], ret[i-1]);
    }
    return ret;
  }
  const int M = (h-1) * 2 + w;
  vector<int> ret(M+1);

  vector<int> reductions;
  int curWidth = w, curHeight = h;
  int covered = 0;
  while(curWidth > 0 && curHeight > 1) {
    covered += curWidth;
    int red = min(curWidth, 2);
    curWidth -= red;
    reductions.push_back(red);
    curHeight -= 1;
  }
  if(curWidth > 0) {
    reductions.push_back(curWidth);
    covered += curWidth;
  }

  // printf("initial reductions: %d stones, covered %d\n", w, covered);
  // print(reductions);
  ret[w] = covered;

  int reduceAt = 0;
  for(int k = w+1; k <= M; ++k) {
    if(reductions[reduceAt] == 0) ++reduceAt;
    reductions[reduceAt]--;
    reductions.back()++;
    covered += reductions.size() - reduceAt - 1;
    if(reductions.size() < h && reductions.back() >= 4) {
      int newLevel = reductions.back() - 2;
      reductions.back() = 2;
      reductions.push_back(newLevel);
      covered += newLevel;
    }

    // printf("with %d stones, covered %d\n", k, covered);
    // print(reductions);
    ret[k] = covered;
  }
  return ret;
}

int main() {
  // vector<int> t = getMaxEnclosure(1, 5);
  // vector<int> t = getMaxEnclosure(5, 2);
  // printf("result:\n");
  // print(t);
  // return 0;
  int cases;
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    int n, m, k;
    cin >> n >> m >> k;
    int ret = n*2+m*2-4;
    for(int h1 = 1; h1 <= n; ++h1) {
      for(int w1 = 1; w1 <= m; ++w1) {
        // printf("h1 = %d w1 = %d\n", h1, w1);
        vector<int> enclose1 = getMaxEnclosure(h1, w1);

        // one sided
        for(int i = 1; i < enclose1.size(); ++i) 
          if(enclose1[i] >= k) 
            ret = min(ret, i + max(w1 - 2, 0));

        // two sided
        if(h1 == n) continue;
        for(int w2 = 1; w2 < w1; ++w2) {
          vector<int> enclose2 = getMaxEnclosure(n - h1, w2);
          for(int from1 = 1; from1 < enclose1.size(); ++from1) {
            int needToCover = k - enclose1[from1];
            if(needToCover <= 0) break;

            if(enclose2.back() < needToCover) continue;

            int from2 = lower_bound(enclose2.begin(), enclose2.end(), needToCover) - enclose2.begin();

            int cand = from1 + from2 + max(w1 - w2 - 2, 0);
            ret = min(cand, ret);
          }
        }
      }
    }
    printf("Case #%d: %d\n", cc+1, ret);
  }
}

