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

int dx[4] = { 0, 1,  0, -1 };
int dy[4] = { 1, 0, -1,  0 };
char d[5] = "NESW";

void move (int dir) {
  printf("%c", d[(dir + 2) % 4]);
  printf("%c", d[dir]);
}

char get (pii a) {
  if  (a.f > 0 && a.s == 1)
    return d[0];
  if  (a.f > 0)
    return  d[1];
  if  (a.f < 0 && a.s == 1)
    return d[2];
  return d[3];
}      

vector <pii> r;

void build (int x, int y) {
  r.clear();

  int c = 0;
  int sum = 0;
  while (sum < x) {
    c++;
    sum += c;
    r.pb(mp(c, 0));
  }
  
  if  (sum > x) {
    if  (sum - x == 3) {
       r[0].f = -1;
       c++;
       r.pb(mp(c, 0));
       c++;
       r.pb(mp(-c, 0));
       sum = 0;  
    }

    forn(i, sz(r))
      if  (r[i].f == (sum - x) / 2) {
        sum -= 2 * r[i].f;
        r[i].f = -r[i].f;
        break;
      }

    if  (sum == x + 1) {
      r[0].s = 1;
      sum = 1;
    } else sum = 0;
  } else sum = 0;

  while (sum < y) {
    c++;

    if  (sum + c > y && y - sum < sum + c - y)
      break;

    sum += c;
    r.pb(mp(c, 1));
  }

  for (int i = 0; i < abs(sum - y); i++)
    if  (sum > y) {
      c++;
      r.pb(mp(c, 1));
      r.pb(mp(-c, 1));
    } else {
      c++;
      r.pb(mp(-c, 1));
      r.pb(mp(c, 1));
    } 
}

int main() {
  freopen("in.txt", "r", stdin);
  freopen("out.txt", "w", stdout);

  int t; scanf("%d", &t);

  for (int test = 1; test <= t; test++) 
  {
    int x, y; scanf("%d %d", &x, &y);

    printf("Case #%d: ", test);

    int res1;
    build(abs(x), abs(y));
    res1 = sz(r);

    int res2;
    build(abs(y), abs(x));
    res2 = sz(r);

    if  (res1 < res2 || true) {
      build(abs(x), abs(y));

      if  (x < 0)
        forn(i, sz(r))
          if  (r[i].s == 0)
            r[i].f = -r[i].f;

      if  (y < 0)
        forn(i, sz(r))
          if  (r[i].s == 1)
            r[i].f = -r[i].f;

      forn(i, sz(r))
        printf("%c", get(r[i]));
    } else {
      forn(i, sz(r))
        r[i].s = 1 - r[i].s;

      if  (x < 0)
        forn(i, sz(r))
          if  (r[i].s == 0)
            r[i].f = -r[i].f;

      if  (y < 0)
        forn(i, sz(r))
          if  (r[i].s == 1)
            r[i].f = -r[i].f;

      forn(i, sz(r))
        printf("%c", get(r[i]));
    }

    puts(""); 
  }

  return 0;
}
