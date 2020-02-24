#include <iostream>
#include <set>
#include <map>
#include <utility>
using namespace std;

typedef pair<int, pair<int, int> > Pos;

int ns[30][30], we[30][30], tt[30][30];
int nc, nr;

Pos make_pos(int cn, int r, int c) {
  return make_pair(cn, make_pair(r, c));
}

Pos walk_north(Pos pos) {
  int cn = pos.first;
  int r = pos.second.first;
  int c = pos.second.second;
  if (cn == 0) return make_pos(1, r-1, c);
  if (cn == 1) return make_pos(0, r, c);
  if (cn == 2) return make_pos(3, r, c);
  if (cn == 3) return make_pos(2, r-1, c);
}

Pos walk_south(Pos pos) {
  int cn = pos.first;
  int r = pos.second.first;
  int c = pos.second.second;
  if (cn == 0) return make_pos(1, r, c);
  if (cn == 1) return make_pos(0, r+1, c);
  if (cn == 2) return make_pos(3, r+1, c);
  if (cn == 3) return make_pos(2, r, c);
}

Pos walk_west(Pos pos) {
  int cn = pos.first;
  int r = pos.second.first;
  int c = pos.second.second;
  if (cn == 0) return make_pos(3, r, c);
  if (cn == 1) return make_pos(2, r, c);
  if (cn == 2) return make_pos(1, r, c-1);
  if (cn == 3) return make_pos(0, r, c-1);
}

Pos walk_east(Pos pos) {
  int cn = pos.first;
  int r = pos.second.first;
  int c = pos.second.second;
  if (cn == 0) return make_pos(3, r, c+1);
  if (cn == 1) return make_pos(2, r, c+1);
  if (cn == 2) return make_pos(1, r, c);
  if (cn == 3) return make_pos(0, r, c);
}

int wait_ns(int r, int c, long long t) {
  long long m = ns[r][c]+we[r][c];
  t = (t-tt[r][c] + m*1000000000LL)%m;
  if (t < ns[r][c]) return 0;
  return m-t;
}

int wait_we(int r, int c, long long t) {
  long long m = ns[r][c]+we[r][c];
  t = (t-tt[r][c] + m*1000000000LL)%m;
  
  if (t >= ns[r][c]) return 0;
  return ns[r][c]-t;
}

int main() {
  int NT;
  cin >> NT;
  for (int C = 1; C <= NT; ++C) {
    cin >> nr >> nc;
    for (int i = 0; i < nr; ++i)
      for (int j = 0; j < nc; ++j)
	cin >> ns[i][j] >> we[i][j] >> tt[i][j];

    // time, corner, pos
    // corner: cw from NE
    set<pair<int, Pos> > q;
    map<Pos, int> dist;

    q.insert(make_pair(0, make_pos(2, nr-1, 0)));
    while (q.size()) {
      int d = q.begin()->first;
      Pos pos = q.begin()->second;
      int cn = pos.first;
      int r = pos.second.first;
      int c = pos.second.second;
      q.erase(q.begin());
      
      if (dist.find(pos) != dist.end() && dist[pos] <= d)
	continue;
      dist[pos] = d;
      if (pos == make_pos(0, 0, nc-1))
	break;

      if (r > 0 && (cn == 0 || cn == 3))
	q.insert(make_pair(d + 2, walk_north(pos)));
      if (c > 0 && (cn == 2 || cn == 3))
	q.insert(make_pair(d + 2, walk_west(pos)));
      if (r < nr-1 && (cn == 1 || cn == 2))
	q.insert(make_pair(d + 2, walk_south(pos)));
      if (c < nc-1 && (cn == 0 || cn == 1))
	q.insert(make_pair(d + 2, walk_east(pos)));

      if (cn == 1 || cn == 2)
	q.insert(make_pair(d + wait_ns(r, c, d) + 1, walk_north(pos)));
      if (cn == 0 || cn == 3)
	q.insert(make_pair(d + wait_ns(r, c, d) + 1, walk_south(pos)));
      if (cn == 2 || cn == 3)
	q.insert(make_pair(d + wait_we(r, c, d) + 1, walk_east(pos)));
      if (cn == 0 || cn == 1)
	q.insert(make_pair(d + wait_we(r, c, d) + 1, walk_west(pos)));
    }

    cout << "Case #" << C << ": " << dist[make_pos(0, 0, nc-1)] << endl;
  }
}
