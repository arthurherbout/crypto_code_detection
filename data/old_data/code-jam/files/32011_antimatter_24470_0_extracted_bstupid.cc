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

int W, H;
int v[2], w[2];
int x, y;
bool hit[100][100];

void go(int x, int y) {
  if (x < 0 || y < 0 || x >= W || y >= H) {
    return;
  }
  
  if (hit[x][y]) return;
  hit[x][y] = 1;
  go(x+v[0], y+v[1]);
  go(x+w[0], y+w[1]);
}

int main() {
  int C; cin >> C;
  for (int it = 1; it <= C; it++) {
    cin >> W >> H;
    cin >> v[0] >> v[1] >> w[0] >> w[1];
    cin >> x >> y;

    memset(hit,0,sizeof(hit));
    go(x,y);

    int ret = 0;
    for (int i = 0; i < W; i++)
      for (int j = 0; j < H; j++)
	if (hit[i][j]) ret++;
    printf("Case #%d: %d\n", it, ret);
  }
}
