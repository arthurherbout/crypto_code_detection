#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdlib>
#define MP make_pair
#define PB push_back

using namespace std;

typedef vector<int> vi;
typedef map<string,int> msi;
typedef pair<int, int> pii;
typedef vector<pii> vpii;
typedef vector<int> VI;
typedef vector<VI> VVI;

bool FindMatch(int i, const VVI &w, VI &mr, VI &mc, VI &seen) {
  for (int j = 0; j < w[i].size(); j++) {
    if (w[i][j] && !seen[j]) {
      seen[j] = true;
      if (mc[j] < 0 || FindMatch(mc[j], w, mr, mc, seen)) {
        mr[i] = j;
        mc[j] = i;
        return true;
      }
    }
  }
  return false;
}

int BipartiteMatching(const VVI &w, VI &mr, VI &mc) {
  mr = VI(w.size(), -1);
  mc = VI(w[0].size(), -1);

  int ct = 0;
  for (int i = 0; i < w.size(); i++) {
    VI seen(w[0].size());
    if (FindMatch(i, w, mr, mc, seen)) ct++;
  }
  return ct;
}


int main() {
	int T;
	cin >> T;
	for(int t = 1;t <= T; ++t) {
		msi a1, a2;
		int n1(0), n2(0);
		int m;
		cin >> m;
		vpii edges;
		for(int i = 0;i < m; ++i) {
			string s, t;
			cin >> s >> t;
			if(a1.count(s) == 0) {
				a1[s] = n1++;
			}
			if(a2.count(t) == 0) {
				a2[t] = n2++;
			}
			edges.PB(MP(a1[s], a2[t]));
		}
		VVI adj(n1, VI(n2, 0));
		for(vpii::iterator it = edges.begin(); it != edges.end(); ++it) {
			++adj[it->first][it->second];
			// ++adj[it->second][it->first];
		}
		VI mr, mc;
		int fisse = BipartiteMatching(adj, mr, mc);
		cout << "Case #" << t << ": " << (m - (n1 + n2 - fisse)) << endl;
	}
	return 0;
}
