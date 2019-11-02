#include <iostream>
#include <set>
#include <utility>
using namespace std;

int A[300], dist[300][300];

int main() {
  int nt, C = 1;
  cin >> nt;
  int dc, ic, md, n;
  while (nt-- && cin >> dc >> ic >> md >> n) {
    for (int i = 0; i < n; ++i)
      cin >> A[i];
    
    set<pair<int, pair<int, int> > > q;
    memset(dist, 1, sizeof dist);
    dist[256][0] = 0, q.insert(make_pair(0, make_pair(256, 0)));
    while (!q.empty()) {
      int d = q.begin()->first;
      int pc = q.begin()->second.first;
      int p = q.begin()->second.second;
      q.erase(q.begin());

      if (dist[pc][p] < d)
	continue;
      dist[pc][p] = d;

      if (p == n)
	continue;

      if (dist[pc][p+1] > d+dc)
	dist[pc][p+1] = d+dc, q.insert(make_pair(d+dc, make_pair(pc, p+1)));
      
      if (pc != 256) {
	for (int i = pc-md; i <= pc+md; ++i)
	  if (i >= 0 && i < 256 && dist[i][p] > d+ic)
	    dist[i][p] = d+ic, q.insert(make_pair(d+ic, make_pair(i, p)));

	for (int i = pc-md; i <= pc+md; ++i)
	  if (i >= 0 && i < 256 && dist[i][p+1] > d+abs(A[p]-i))
	    dist[i][p+1] = d+abs(A[p]-i), q.insert(make_pair(d+abs(A[p]-i), make_pair(i, p+1)));
      } else {
	for (int i = 0; i <= 255; ++i)
	  if (dist[i][p+1] > d+abs(A[p]-i))
	    dist[i][p+1] = d+abs(A[p]-i), q.insert(make_pair(d+abs(A[p]-i), make_pair(i, p+1)));
      }
    }

    int best = 1000000000;
    for (int i = 0; i <= 256; ++i)
      best = min(best, dist[i][n]);
    cout << "Case #" << C++ << ": " << best << endl;
  }
}
