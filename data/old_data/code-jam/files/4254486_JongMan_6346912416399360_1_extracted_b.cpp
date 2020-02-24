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

int n, k;
long long sum[10000];
long long delta[10000];

int main() {
  int cases;
  cin.sync_with_stdio(false);
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {

    cin >> n >> k;

    for(int i = 0; i < n-k+1; ++i) 
      cin >> sum[i];

    vector<pair<long long, long long> > heads;

    long long lb = 0, ub = 0;
    for(int i = 0; i < k; ++i) {
      delta[i] = 0;
      long long mini = 0, maxi = 0;
      for(int j = i; j + k < n; j += k) {
        delta[j+k] = sum[j+1] - sum[j] + delta[j];
        mini = min(mini, delta[j+k]);
        maxi = max(maxi, delta[j+k]);
      }

      heads.push_back(make_pair(mini, maxi));
      lb = min(lb, mini);
      ub = max(ub, maxi);
    }

    long long ret = 987654321;
    // for(int i = 0; i < 100; ++i)
    //   printf("delta[%d] = %lld\n", i, delta[i]);
    fprintf(stderr, "search range %lld ub %lld lb %lld\n", 
            max(0ll, sum[0]) + ub - lb + min(0ll, sum[0]), ub, lb);
    for(long long lower = min(0ll, sum[0]) + lb; lower <= max(0ll, sum[0]) + ub; ++lower) {
      vector<long long> tops;
      long long req = sum[0];
      for(auto a: heads) {
        req -= lower - a.first;
        tops.push_back(lower + a.second - a.first);
      }

      if(req < 0) {
        // printf("lower = %lld is impossible\n", lower);
        break;
      }
      // printf("lower = %lld =>", lower);
      // for(long long t: tops) {
      //   printf(" %lld", t);
      // }
      // printf("\n");
      long long tallest = *max_element(tops.begin(), tops.end());
      long long headroom = 0;
      for(long long t: tops) headroom += tallest - t;
      // printf("headroom is %lld, req %lld\n", headroom, req);
      req -= min(req, headroom);

      long long upper = tallest + (req + (k - 1)) / k;

      // printf("upper = %lld + %lld\n", tallest, (req + k - 1) / k);

      ret = min(ret, upper - lower);

    }

    cout << "Case #" << (cc+1) << ": " << ret << endl;
  }
}

