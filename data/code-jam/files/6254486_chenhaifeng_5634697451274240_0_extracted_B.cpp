#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <stdint.h>
#include <limits.h>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

string flipPans(const string& pans, const size_t& len) {
  string new_pans = pans.substr(0, len);
  reverse(new_pans.begin(), new_pans.end());
  for (size_t i = 0; i < new_pans.size(); ++i) {
    new_pans[i] = new_pans[i] == '+' ? '-' : '+';
  }
  new_pans += pans.substr(len);
  return new_pans;
}

bool check_all_pos(const string& pans) {
  for (size_t i = 0; i < pans.size(); ++i) {
    if (pans[i] == '-') {
      return false;
    }
  }
  return true;
}

int bfs(const string& pans) {
  if (check_all_pos(pans))
    return 0;
  unordered_set<string> visited;
  queue<string> q;
  visited.insert(pans);
  q.push(pans);
  int lev = 0;
  while (!q.empty()) {
    lev++;
    int s = q.size();
    for (int i = 0; i < s; ++i) {
      string f = q.front();
      q.pop();

      for (size_t j = 1; j <= f.size(); ++j) {
        string new_pans = flipPans(f, j);
        if (check_all_pos(new_pans)) {
          return lev;
        }
        if (visited.find(new_pans) == visited.end()) {
          visited.insert(new_pans);
          q.push(new_pans);
        }
      }
    }
  }
  return INT_MAX;
}

int main() {
  freopen("B.in", "r", stdin);
  freopen("B.out", "w", stdout);
  size_t T;
  cin >> T;
  for (size_t t = 1; t <= T; ++t) {
    string pans;
    cin >> pans;
    printf("Case #%d: %d\n", t, bfs(pans));
  }
  return 0;
}
