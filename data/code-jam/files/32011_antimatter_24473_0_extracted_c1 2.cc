#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <fstream>
using namespace std;

typedef long long ii;

int R, C;
bool h[5][5];
int g[5][5];

bool ok() {
  for (int i = 0; i < R; i++)
    for (int j = 0; j < C; j++) {
      int ct = 0;
      for (int k = -1; k <= 1; k++)
	for (int m = -1; m <= 1; m++) {
	  int tx = i+k, ty = j+m;
	  if (tx < 0 || ty < 0 || tx >= R || ty >= C) continue;
	  if (h[tx][ty]) ct++;
	}
      if (ct != g[i][j]) return 0;
    }
  return 1;
}
int main() {
  int N;
  cin >> N;
  for (int it = 1; it <= N; it++) {
    cin >> R >> C;
    for (int i = 0; i < R; i++)
      for (int j = 0; j < C; j++)
	cin >> g[i][j];

    int ret = 0;
    for (int i = 0; i < (1 << (R*C)); i++) {
      for (int j = 0; j < R*C; j++) {
	int x = j/C, y = j%C;
	h[x][y] = (i & (1 << j));
      }

      if (ok()) {
	int mid = R/2;
	int ct = 0;
	for (int k = 0; k < C; k++) if (h[mid][k]) ct++;
	if (ct > ret) ret = ct;
      }
    }
    printf("Case #%d: %d\n", it, ret);
  }
}
