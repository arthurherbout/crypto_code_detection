#include <string>
#include <stdio.h>
#include <vector>
#include <map>
#include <set>
#include <iostream>
using namespace std;
struct zeroint {
  int a;
  zeroint() { a = 0; }
  operator int &() { return a; }
};
int main() {
  int Tn, Ti;
  cin >> Tn;
  for (Ti = 1; Ti <= Tn; ++Ti) {
    map<string, zeroint> first;
    map<string, zeroint> second;
    vector<pair<string, string>> topic;
    map<string, vector<string>> ffs;
    map<string, vector<string>> fss;
    set<string> ff;
    set<string> nf;
    set<string> fs;
    set<string> ns;
    int n;
    cin >> n;
    while (n--) {
      string f, s;
      cin >> f >> s;
      ++first[f];
      ++second[s];
      topic.push_back({f, s});
    }
    int res = 0;
    for (auto &it : topic) {
      if (first[it.first] > 1 && second[it.second] > 1)
        ++res, ff.insert(it.first), fs.insert(it.second),
            ffs[it.first].push_back(it.second),
            fss[it.second].push_back(it.first);
      else
        nf.insert(it.first), ns.insert(it.second);
    }
    for (auto &it : ff) {
      if (nf.count(it))
        continue;
      for (auto &jt : ffs[it]) {
        if (ns.count(jt))
          continue;
        ns.insert(jt);
        break;
      }
      nf.insert(it);
      --res;
    }
    for (auto &it : fs) {
      if (ns.count(it))
        continue;
      for (auto &jt : fss[it]) {
        if (nf.count(jt))
          continue;
        nf.insert(jt);
        break;
      }
      ns.insert(it);
      --res;
    }
    cout << "Case #" << Ti << ": " << res << "\n";
  }
}