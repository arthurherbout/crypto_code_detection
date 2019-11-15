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

int n;
int my_speed;
int pos[1000];
int speed[1000];

int l, r;
int rpos[1000], rspeed[1000];
int lpos[1000], lspeed[1000];
double r_ttc[1000];

double time_to_catch(double speed, double pos) {
  // (pos + x) / my_speed == x / speed
  double x = (pos * speed) / (my_speed - speed);
  return x / speed;
}

double go() {
  double lo = 0, hi = 1e20;
  l = r = 0;
  for(int i = 0; i < n; ++i) {
    if(pos[i] < 0) {
      lpos[l] = abs(pos[i]);
      lspeed[l] = speed[i];
      ++l;
    }
    else {
      rpos[r] = pos[i];
      rspeed[r] = speed[i];
      r_ttc[r] = time_to_catch(speed[i], pos[i]);
      ++r;
    }
  }

  bool c = false;

  for(int it = 0; it < 50; ++it) {
    double mid = (lo + hi) / 2;
    bool good = false;
    for(int c = 0; c <= r; ++c) {
      double start_time = 0;
      if(c > 0) start_time = r_ttc[c-1];

      double max_left = 0;
      for(int i = 0; i < l; ++i) {
        max_left = max(max_left, time_to_catch(lspeed[i], lpos[i] + start_time * (lspeed[i] + my_speed)));
      }

      double pos_reversion = (start_time - max_left) * my_speed;
      double max_right = 0;
      for(int i = 0; i < r; ++i) {
        if(start_time < r_ttc[i]) {
          max_right = max(max_right,
                          time_to_catch(rspeed[i], rpos[i] + (rspeed[i] * (start_time + max_left)) - pos_reversion));
        }
      }

      if(start_time + max_left + max_right <= mid) {
        good = true;
        break;
      }
    }
    if(good) {
      hi = mid;
      c = true;
    }
    else {
      lo = mid;
    }
  }

  assert(c);
}

double solve() {
  double ret = go();
  for(int i = 0; i < n; ++i) 
    pos[i] = -pos[i];
  ret = min(ret, go());
  return ret;
}

int main() {
  int cases;
  cin.sync_with_stdio(false);
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    cin >> my_speed >> n;
    for(int i = 0; i < n; ++i) 
      cin >> pos[i];
    for(int i = 0; i < n; ++i) 
      cin >> speed[i];

      // printf("Ttc pos %d speed %d => %.10lf\n", pos[i], speed[i], time_to_catch(speed[i], pos[i]));
    printf("Case #%d: %.10lf\n", cc+1, solve());
  }
}

