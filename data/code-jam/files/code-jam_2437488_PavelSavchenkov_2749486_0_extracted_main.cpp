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

int main() {
  freopen("in.txt", "r", stdin);
  freopen("out.txt", "w", stdout);
  int t; scanf("%d", &t);
  for (int test = 1; test <= t; test++) {
    int x, y; scanf("%d %d", &x, &y);

    printf("Case #%d: ", test);

    for (int i = 0; i < abs(x); i++)
      move(x < 0 ? 3 : 1);
    for (int i = 0; i < abs(y); i++)
      move(y < 0 ? 2 : 0);
    puts("");
  }
  return 0;
}
