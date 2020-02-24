#include <bits/stdc++.h>

using namespace std;

vector< pair<string, string> > pairs;

int main() {
  ios_base::sync_with_stdio(false);
  int nt; cin >> nt;
  for (int caso = 1; caso <= nt; ++caso) {
    pairs.clear();

    int n; cin >> n;
    for (int i = 0; i < n; ++i) {
      string left_str, right_str; cin >> left_str >> right_str;
      pairs.push_back(make_pair(left_str, right_str));
    }

    int answer = 0;

    for (int mask = 0, end = 1 << n; mask < end; ++mask) {
      vector< pair<string, string> > original, copy;

      for (int i = 0; i < n; ++i) {
        if ((mask >> i) & 1) original.push_back(pairs[i]);
        else copy.push_back(pairs[i]);
      }

      unordered_set<string> right_side, left_side;

      for (int i = 0; i < original.size(); ++i) {
        left_side.insert(original[i].first);
        right_side.insert(original[i].second);
      }

      bool valid = true;

      for (int i = 0; i < copy.size(); ++i) {
        if (left_side.find(copy[i].first) == left_side.end()) valid = false;
        if (right_side.find(copy[i].second) == right_side.end()) valid = false;
        if (!valid) break;
      }

      if (valid) answer = max(answer, (int) copy.size());
    }

    cout << "Case #" << caso << ": " << answer << "\n";
  }
  return 0;
}