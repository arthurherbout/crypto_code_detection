#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <math.h>
#include <cstdlib>
#include <memory.h>
#include <queue>
#include <assert.h>
#include <cmath>

using namespace std;

#define pb push_back
#define f first
#define s second
#define mp make_pair 
#define sz(A) ((int)(A).size())
#define forn(i, n) for (int i = 0; i < int(n); i++)
#define ford(i, n) for (int i = int(n) - 1; i >= 0; i--)
#define foran(i, a, n) for (int i = int(a); i < int(n); i++)
#define y1 gftxdtrtfhyjfctrxujkvbhyjice
#define y0 ehfoiuvhefroerferjhfjkehfjke

typedef long long ll;
typedef unsigned long long ull;
typedef pair <int,int> pii;

const double EPS = 1e-9;
const int MAXM = 1010;
const int MOD = 1000002013;

struct pass {
  int a, b, cnt;

  pass () {}

  void read () {
    scanf("%d %d %d", &a, &b, &cnt);
  }

  friend bool operator < (const pass & x, const pass & y) {
    return x.a < y.a;
  }
};

int add (int a, int b) {
  a += b;
  if  (a >= MOD) a -= MOD;
  return a;
}

int mul (int a, int b) {
  ll M = 1ll * a * b;
  M %= ll(MOD);
  return int(M);
}

int mpow (int a, int p) {
  int ans = 1;
  while (p > 0) {
    if  (p & 1)
      ans = mul(ans, a);
    a = mul(a, a);
    p /= 2;
  }
  return ans;
}

multiset <pass> s[2];
pass p[MAXM];
int n, m;

int get (int c) {
  int ans = add(n, n);
  ans = add(ans, 1);
  ans = add(ans, (-c + MOD) % MOD);
  ans = mul(ans, mpow(2, 970214400 - 1));
  return mul(ans, c);
}

int main() {
  freopen("in.txt", "r", stdin);
  freopen("out.txt", "w", stdout);
  int t; scanf("%d", &t);

  for (int test = 1; test <= t; test++) 
  {
    scanf("%d %d", &n, &m);
    int all = 0;
    forn(i, m) {
      p[i].read();
      all = add(all, mul(get(p[i].b - p[i].a), p[i].cnt));
    }

    sort(p, p + m);

    int c = 0; s[0].clear(); s[1].clear();
    for (int i = 0; i < m; i++, c = 1 - c) 
    {
//      printf("sz = %d\n", sz(s[c]));
      for (multiset <pass> :: iterator it = s[c].begin(); it != s[c].end(); it++) {
        pass cur;
        cur.a = it->a;
        cur.b = it->b;
        cur.cnt = it->cnt;

        if  (cur.b < p[i].a || p[i].b <= cur.b) {
          s[1 - c].insert(cur);
          continue;
        }

        int toswap = min(p[i].cnt, cur.cnt);
        p[i].cnt -= toswap;
        cur.cnt -= toswap;

        pass n1;
        n1.a = p[i].a;
        n1.b = cur.b;
        n1.cnt = toswap;

        pass n2;
        n2.a = cur.a;
        n2.b = p[i].b;
        n2.cnt = toswap;

        if  (n1.cnt > 0)
          s[1 - c].insert(n1);
        if  (n2.cnt > 0)
          s[1 - c].insert(n2);
        if  (cur.cnt > 0)
          s[1 - c].insert(cur);
//        printf("i = %d cur = [%d %d]\n", i, cur.a, cur.b);
      }

      s[c].clear();
      if  (p[i].cnt > 0)
        s[1 - c].insert(p[i]);

//      for (multiset <pass> :: iterator it = s[1 - c].begin(); it != s[1 - c].end(); it++)
//        printf("%d %d %d\n", it->a, it->b, it->cnt);
//      puts("");  
    }

    if  (s[0].empty())
      c = 1;
    else
      c = 0;

    int ans = 0;
    for (multiset <pass> :: iterator it = s[c].begin(); it != s[c].end(); it++) {
      ans = add(ans, mul(get(it->b - it->a), it->cnt));
//      printf("%d -> %d %d\n", it->a, it->b, it->cnt);
    }

//    printf("%d %d\n", n, get(1));
    all -= ans;
    while (all < 0) all += MOD;
    printf("Case #%d: %d\n", test, all);
  }

  return 0;
}
