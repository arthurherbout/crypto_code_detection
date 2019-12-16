#include <bits/stdc++.h>
#define st first
#define nd second

using namespace std;

typedef pair<int,int> ii;
typedef long long ll;

struct PT {
  ll x, y;
  PT() {}
  PT(ll x, ll y) : x(x), y(y){}
  PT(const PT &p) : x(p.x), y(p.y){}
  PT operator + (const PT &p)     const { return PT(x+p.x,y+p.y); }
  PT operator - (const PT &p)     const { return PT(x-p.x,y-p.y); }
  ll operator * (const PT &p) const { return x*p.x+y*p.y;     }
  ll operator % (const PT &p) const { return x*p.y-y*p.x;     }
  bool operator < (const PT& p) const {
    if (x != p.x) return x < p.x;
    return y < p.y;
  }
};

bool comp1(PT a, PT b) {
  return a.x < b.x || a.x == b.x && a.y < b.y;
}

bool comp2(PT a, PT b) {
  return a%b > 0 || a%b == 0 && a*a < b*b;
}

vector<PT> hull(vector<PT> p) {
  int n = p.size();
  if (n < 3) return p;
  PT o = *min_element(p.begin(), p.end(), comp1);
  for (int i = 0; i < n; ++i) {
    p[i] = p[i] - o;
  }
  sort(p.begin(), p.end(), comp2);
  int w = n-1;
  while (w && p[w] % p[w-1] == 0) w--;
  reverse(p.begin() + w, p.end());
  int m = 0;
  for (int i = 1; i < n; ++i) {
    while (m && (p[i] - p[m-1]) % (p[m] - p[m-1]) > 0) --m;
    p[++m] = p[i];
  }
  p.resize(++m);
  for (int i = 0; i < m; ++i) p[i] = p[i] + o;
  return p;
}

int ans[20];

int main() {
  int nt; scanf("%d", &nt);
  for (int _ = 1; _ <= nt; ++_) {
    int n; scanf("%d", &n);
    vector<PT> p;
    map<PT, int> id;
    for (int i = 0; i < n; ++i) {
      int x, y; scanf("%d %d", &x, &y);
      p.push_back(PT(x,y));
      id[PT(x,y)] = i;
    }
    memset(ans, 0x3f, sizeof ans);
    for (int mask = 1, e = 1 << n; mask < e; ++mask) {
      vector<PT> aux;
      for (int i = 0; i < n; ++i) if (mask & (1 << i)) {
        aux.push_back(p[i]);
      }
      //aux.push_back(aux[0]);
      vector<PT> h = hull(aux);
      for (int i = 0; i < h.size(); ++i) {
        map<PT,int>::iterator it = id.find(h[i]);
        assert(it != id.end());
        int x = it->nd;
        ans[x] = min(ans[x], n - __builtin_popcount(mask));
      }
    }
    printf("Case #%d:\n", _);
    for (int i = 0; i < n; ++i) printf("%d\n", ans[i]);
    fprintf(stderr, "case %d finished\n", _);
  }
  return 0;

}