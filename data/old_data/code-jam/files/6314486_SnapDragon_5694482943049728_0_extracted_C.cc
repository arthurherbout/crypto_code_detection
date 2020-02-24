#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;

int tot = 0;
void span(int i, const VI& comp, const vector<pair<int, int>>& e) {
  if (i == e.size()) {
    bool done = true;
    for (int i = 1; i < comp.size(); i++) if (comp[i] != comp[0]) { done = false; break; }
    if (done) tot++;
    return;
  }
//cout << i << ' ' << e.size() << ' ' << e[i].first << ' ' << e[i].second << ' ' << comp.size() << endl;
  if (tot > 10000) return;
  if (comp[e[i].first] == comp[e[i].second]) {
    span(i+1, comp, e);
    return;
  }

  VI comp2 = comp;
  for (int j = 0; j < comp2.size(); j++) if (comp2[j] == comp[e[i].second]) comp2[j] = comp[e[i].first];
  int otot = tot;
  span(i+1, comp2, e);
  if (tot == otot) return;
  span(i+1, comp, e);
}

int seen[10001];
vector<string> g[10001];

int main() {
  srand(time(0));
  int read = 0;
  {
    ifstream fin("C.data");
    int k, n;
    while (fin >> k >> n) {
      if (!seen[k]) {
        seen[k] = true;
        read++;
      }
      g[k].resize(n);
      for (int i = 0; i < n; i++) fin >> g[k][i];
    }
    cerr << "Done reading " << read << endl;
  }

  /*for (;;) {
    int nd = rand()%7 + 9;
    vector<pair<int, int>> e;
    VVI c(nd, VI(nd));
    int ne = rand()%(nd*(nd-1)/2);
    for (int i = 0; i < ne; i++) {
      int x = rand()%nd, y = rand()%nd;
      if (x == y) { i--; continue; }
      if (c[x][y]) { i--; continue; }
      c[x][y] = c[y][x] = 1;
      e.push_back(make_pair(min(x, y), max(x, y)));
    }
    //for (int i = 0; i < nd; i++) {
      //int x = i, y = (i+1)%nd;
      //c[x][y] = c[y][x] = 1;
      //e.push_back(make_pair(min(x, y), max(x, y)));
    //}

    tot = 0;
    VI comp(nd);
    for (int i = 0; i < nd; i++) comp[i] = i;
    sort(e.begin(), e.end());
    span(0, comp, e);
    if (tot != 0 && tot <= 10000 && !seen[tot]) {
      cerr << "Found " << tot << endl;
      seen[tot] = true;
      cout << tot << ' ' << nd << endl;
      for (int y = 0; y < nd; y++) {
        for (int x = 0; x < nd; x++) cout << c[y][x];
        cout << endl;
      }
    }
  }*/

  int T, prob=1;
  for (cin >> T; T--;) {
    int k;
    cin >> k;
    if (!seen[k]) {
      cerr << "Missing " << k << endl;
      cout << "Case #" << prob++ << ": MISSING";
    } else {
      cout << "Case #" << prob++ << ": " << g[k].size() << endl;
      for (int i = 0; i < g[k].size(); i++) cout << g[k][i] << endl;
    }
  }
}
