#include "assert.h"
#include "ctype.h"
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "time.h"
#include "algorithm"
#include "numeric"
#include "functional"
#include "bitset"
#include "vector"
#include "list"
#include "set"
#include "map"
#include "queue"
#include "stack"
#include "string"
#include "sstream"
#include "iostream"
using namespace std;

#ifndef ONLINE_JUDGE
#pragma warning(disable:4786)  // long identifiers
#pragma warning(disable:4996)  // deprecations
#endif

typedef long long i64;

//////////////////////////////////////////////////////////////////////////////////////////

struct node_t {
  vector<int> children;
};

map<string, int> nameMap;
node_t tree[2000];
int m = 0, current, best;

int GetItem(const string& name) {
  const map<string, int>::const_iterator it = nameMap.find(name);
  if (it != nameMap.end()) return it->second;
  const int index = m++;
  tree[index].children.clear();
  nameMap.insert(make_pair(name, index));
  return index;
}

int f(int node) {
  vector<int> v;
  for (int i = 0; i < (int)tree[node].children.size(); ++i)
    v.push_back(f(tree[node].children[i]));
  sort(v.begin(), v.end(), greater<int>());
  int best = (int)tree[node].children.size() + 1;
  for (int i = 0; i < (int)v.size(); ++i)
    best = max(best, v[i] + i);
  return best;
}

int main() {
#ifndef ONLINE_JUDGE
  freopen("data.in", "r", stdin);
  freopen("data.out", "w", stdout);
#endif
  int T; scanf("%d", &T);
  for (int Ti = 1; Ti <= T; ++Ti) {
    nameMap.clear(); m = 0;
    int n; scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
      static char name[300]; scanf("%s", name);
      const int index = GetItem(string(name));
      int q; scanf("%d", &q);
      for (int j = 0; j < q; ++j) {
        static char subName[300]; scanf("%s", subName);
        if (isupper(subName[0])) {
          const int subIndex = GetItem(string(subName));
          tree[index].children.push_back(subIndex);
        }
      }
    }
    printf("Case #%d: %d\n", Ti, f(0));
  }
  return 0;
}
