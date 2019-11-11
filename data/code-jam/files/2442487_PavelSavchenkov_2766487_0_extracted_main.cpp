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

int n;
ll p;

bool always (ll a) {
  int cnt = 0;
  while (1) {
    ll val = 1ll << cnt;
    val--;

    if  (val > a) {
      cnt--;
      break;
    }

    cnt++;
  }

  ll m = (1ll << n) / (1 << cnt);
  return p + m > (1ll << n);
}

bool can (ll a) {
  int cnt = 0;
  while (1) {
    ll val = 1ll << cnt;
    val--;

//    printf("val = %I64d\n", val);
    if  (val > (1ll << n) - a - 1ll) {
      cnt--;
      break;
    }

    cnt++;
  }
  
//  printf("cnt = %d\n", cnt);

  ll m = (1ll << n) / (1ll << cnt);
  return m <= p; 
}

int main() {
  freopen("in.txt", "r", stdin);
  freopen("out.txt", "w", stdout);
//  n = 3;
//  can(6ll);
//        return 0;

  int t; scanf("%d", &t);
  for (int test = 1; test <= t; test++) {
    scanf("%d %I64d", &n, &p); 

    ll L = 0; ll R = 1ll << n;
    while (L != R - 1ll) {
      ll M = (L + R) / 2ll;

      if  (always(M))
        L = M;
      else
        R = M;
    }  

    printf("Case #%d: %I64d ", test, L);
 
    L = 0; R = 1ll << n;
    while (L != R - 1ll) {
      ll M = (L + R) / 2ll;
      if  (can(M))
        L = M;
      else
        R = M;
    }

    printf("%I64d\n", L);
  }

  return 0;
}
