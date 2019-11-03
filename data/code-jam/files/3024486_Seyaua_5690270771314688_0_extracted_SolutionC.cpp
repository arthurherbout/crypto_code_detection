#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <bitset>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>
#include <vector>
#include <ctime>
#pragma comment(linker, "/STACK:256000000")

using namespace std;

const int maxN = 1100;
string cur[maxN];
int id[maxN];
int n;
int res;

int dec(int num, int bt) {
  if (num & (1 << bt)) {
    num -= 1 << bt;
  }
  return num;
}

int inc(int num, int bt) {
  return num | (1 << bt);
}

void rec(int num, int havein, int haveout, int lnum) {
  if (num == n) {
    bitset<40> b = havein;
    res = min(res, (int)b.count());
    return;
  }

  if (cur[num] == "L") {
    if (id[num] == 0) {
      for (int i = 1; i <= lnum + 1; ++i) {
        if (!(haveout & (1 << i))) {
          rec(num + 1, dec(havein, i), inc(haveout, i), max(i, lnum));
        }
      }
    } else {
      if (haveout & (1 << id[num])) {
        return;
      }
      rec(num + 1, dec(havein, id[num]), inc(haveout, id[num]), lnum);
    }
  } else {
    if (id[num] == 0) {
      for (int i = 1; i <= lnum + 1; ++i) {
        if (!(havein & (1 << i))) {
          rec(num + 1, inc(havein, i), dec(haveout, i), max(i, lnum));
        }
      }
    } else {
      if (havein & (1 << id[num])) {
        return;
      }
      rec(num + 1, inc(havein, id[num]), dec(haveout, id[num]), lnum);
    }
  }
}

void solve(int tcase) {
  cin >> n;
  vector <int> xx;
  xx.push_back(0);
  for (int i = 0; i < n; ++i) {
    cin >> cur[i] >> id[i];
    xx.push_back(id[i]);
  }
  sort(xx.begin(), xx.end());
  xx.resize(unique(xx.begin(), xx.end()) - xx.begin());
  map<int, int> gox;
  for (int i = 0; i < xx.size(); ++i) {
    gox[xx[i]] = i;
  }
  for (int i = 0; i < n; ++i) {
    id[i] = gox[id[i]];
  }

  res = 10000;
  rec(0, 0, 0, xx.size());
  if (res == 10000) {
    printf("Case #%d: CRIME TIME\n", tcase);
  } else {
    printf("Case #%d: %d\n", tcase, res);
  }
}

int main() {
  freopen("input.txt", "r", stdin);
  freopen("output.txt", "w", stdout);

  int t;
  cin >> t;

  for (int i = 1; i <= t; ++i) {
    solve(i);
    cerr << i << endl;
  }

  return 0;
}
