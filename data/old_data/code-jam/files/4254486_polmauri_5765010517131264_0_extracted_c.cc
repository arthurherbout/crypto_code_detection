#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <utility>
#include <vector>
using namespace std;

typedef long long ll;
typedef long double ld;
typedef pair<int, int> P;
typedef vector<int> Vi;
typedef vector<Vi> Mi;
typedef vector<P> Vp;

const ld INF = 1e100;
const ld EPS = 1e-8;

Vp vneg, vpos;
int Y;

int sign(int x) {
  if (x < 0) return -1;
  if (x > 0) return 1;
  return 0;
}

bool discards(P a, P b, ld t) {
  if (sign(a.first) != sign(a.second)) return false;
  ld pa = abs(a.first) + a.second*t;
  ld pb = abs(b.first) + b.second*t;
  ld sa = a.second;
  ld sb = b.second;
  return pa + EPS >= pb and sa > sb + EPS;
}

Vp filter(const Vp& v, ld t) {
  int n = v.size();
  vector<bool> discard(n, false);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i == 0) continue;
      if (discards(v[j], v[i], t)) {
        discard[i] = true;
        break;
      }
    }
  }
  Vp u;
  for (int i = 0; i < n; ++i) {
    if (not discard[i]) u.push_back(v[i]);
  }
  return u;
}

Vp filter_more(const Vp& v, ld p, ld t) {
  Vp u;
  for (int i = 0; i < int(v.size()); ++i) {
    ld pos = v[i].first + sign(v[i].second)*v[i].second*t;
    if (p > 0 and 0 < pos and pos <= p + EPS) continue;
    if (p < 0 and p <= pos + EPS and pos < 0) continue;
    u.push_back(v[i]);
  }
  return u;
}

int get_first(const Vp& v, ld t) {
  ld mini = INF;
  int pos = -1;
  for (int i = 0; i < int(v.size()); ++i) {
    ld p = abs(v[i].first) + t*v[i].second;
    if (p < mini) {
      mini = p;
      pos = i;
    }
  }
  return pos;
}

Vp remove(const Vp& v, int pos) {
  Vp u;
  for (int i = 0; i < int(v.size()); ++i) {
    if (i != pos) {
      u.push_back(v[i]);
    }
  }
  return u;
}

ld fun(ld p, ld t, Vp neg, Vp pos, int step = 0) {
  // cerr << "pos=" << pos.size() << " neg=" << neg.size() << endl;

  neg = filter_more(filter(neg, t), p, t);
  pos = filter_more(filter(pos, t), p, t);

  // neg = filter_more(neg, p, t);
  // pos = filter_more(pos, p, t);

  // cerr << "filter pos=" << pos.size() << " neg=" << neg.size() << endl;
  if (int(neg.size()) == 0 and int(pos.size()) == 0) {
    // cerr << "step=" << step << endl;
    return t;
  }
  ld res = INF;
  if (neg.size()) {
    int f = get_first(neg, t);
    ld p0 = neg[f].first;
    ld s = neg[f].second;
    ld T = (p - p0 + Y*t)/(Y - s);
    ld P = p0 - s*T;
    if (T + EPS < t) return INF;
    // if (T <= t) {
    //   cerr << "t=" << t << endl;
    //   cerr << "p=" << p << endl;
    //   cerr << "T=" << T << endl;
    //   cerr << "Y=" << Y << endl;
    //   cerr << "vneg[neg]= " << vneg[neg].first << " " << vneg[neg].second << endl;
    // }
    assert(T + EPS >= t);
    assert(P <= p + EPS);
    res = min(res, fun(P, T, remove(neg, f), pos, step + 1));
  }
  if (pos.size()) {
    int f = get_first(pos, t);
    ld p0 = pos[f].first;
    ld s = pos[f].second;
    ld T = (p0 - p + Y*t)/(Y - s);
    ld P = p0 + s*T;
    if (T + EPS < t) return INF;
    assert(T + EPS >= t);
    assert(P >= p + EPS);
    res = min(res, fun(P, T, neg, remove(pos, f), step + 1));
  }
  return res;
}

// ld fun(ld p, ld t, int neg, int pos) {
//   if (neg == int(vneg.size()) and pos == int(vpos.size())) {
//     return t;
//   }
//   ld res = INF;
//   if (neg < int(vneg.size())) {
//     ld p0 = vneg[neg].first;
//     ld s = vneg[neg].second;
//     ld T = (p - p0 + Y*t)/(Y - s);
//     ld P = p0 - s*T;
//     if (T <= t) {
//       cerr << "t=" << t << endl;
//       cerr << "p=" << p << endl;
//       cerr << "T=" << T << endl;
//       cerr << "Y=" << Y << endl;
//       cerr << "vneg[neg]= " << vneg[neg].first << " " << vneg[neg].second << endl;
//     }
//     assert(T > t);
//     assert(P < p);
//     res = min(res, fun(P, T, neg + 1, pos));
//   }
//   if (pos < int(vpos.size())) {
//     ld p0 = vpos[pos].first;
//     ld s = vpos[pos].second;
//     ld T = (p0 - p + Y*t)/(Y - s);
//     ld P = p0 + s*T;
//     assert(T > t);
//     assert(P > p);
//     res = min(res, fun(P, T, neg, pos + 1));
//   }
//   return res;
// }

bool sortf(P a, P b) {
  return abs(a.first) < abs(b.first);
}

int main() {
  cout.setf(ios::fixed);
  cout.precision(8);
  int tcas;
  cin >> tcas;
  for (int cas = 1; cas <= tcas; ++cas) {
    int n;
    cin >> Y >> n;
    Vp v(n);
    for (int i = 0; i < n; ++i) {
      cin >> v[i].first;
    }
    for (int i = 0; i < n; ++i) {
      cin >> v[i].second;
    }
    n = v.size();
    Vp neg, pos;
    for (int i = 0; i < n; ++i) {
      if (v[i].first < 0) {
        neg.push_back(v[i]);
      } else {
        pos.push_back(v[i]);
      }
    }
    // cerr << "vneg:" << endl;
    // for (int i = 0; i < int(vneg.size()); ++i)
    //   cerr << " (" << vneg[i].first << ", " << vneg[i].second << ")";
    // cerr << endl;

    // cerr << "vpos:" << endl;
    // for (int i = 0; i < int(vpos.size()); ++i)
    //   cerr << " (" << vpos[i].first << ", " << vpos[i].second << ")";
    // cerr << endl;

    cout << "Case #" << cas << ": " << fun(0, 0, neg, pos) << endl;
  }
}
