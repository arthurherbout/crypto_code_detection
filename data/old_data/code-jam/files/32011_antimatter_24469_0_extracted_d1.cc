#include <cstdio>
#include <queue>
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

//30x30
int N, M;
char grid[31][31];
int NC;
int camp[900][2];

int d[30][30][900]; // x, y -> camp c
bool hit[31][31];

struct foo {int x, y, d;};
void bfs(int x, int y, int c) {
  foo start={x,y,0};
  queue<foo> q;
  q.push(start);
  d[x][y][c] = 0;

  while (q.size()) {
    foo f = q.front(); q.pop();
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
	if (i && j) continue;
	if (!i && !j) continue;

	int tx = f.x + i, ty = f.y + j;
	if (tx < 0 || ty < 0 || tx >= N || ty >= M) continue;
	if (grid[tx][ty] == '.') continue;
	if (d[tx][ty][c] < 505290270) continue;

	d[tx][ty][c] = f.d + 1;
	foo temp = {tx,ty,f.d+1};
	q.push(temp);
      }
    }
  }
}

int main() {
  int T; scanf("%d", &T);
  for (int it = 1; it <= T; it++) {
    scanf("%d %d", &N, &M);
    for (int i = 0; i < N; i++)
      scanf("%s", grid[i]);

    NC = 0;
    for (int i = 0; i < N; i++)
      for (int j = 0; j < M; j++) {
	if (grid[i][j] == 'T') {
	  camp[NC][0] = i;
	  camp[NC][1] = j;
	  NC++;
	}
      }

    memset(d,30,sizeof(d));
    for (int i = 0; i < NC; i++) {
      bfs(camp[i][0], camp[i][1], i);
    }


    // in small, we only have one other forest at most
    if (NC == 1) {
      int ret = 0;
      for (int i = 0; i < N; i++)
	for (int j = 0; j < M; j++)
	  if (grid[i][j] != '.')
	    ret += d[i][j][0];
      printf("Case #%d: %d\n", it, ret);
    }
    else {
      /*
      for (int i = 0; i < NC; i++, printf("\n\n"))
	for (int j = 0; j < N; j++, printf("\n"))
	  for (int k = 0; k < M; k++)
	    printf("%d ", d[j][k][i]);
      */

      // get to the other forest, once
      int ret = 0;

      vector<pair<int,int> > S;

      queue<foo> Q;
      foo start = {0,0,0};
      Q.push(start);
      int maxd = d[0][0][1];
      while (Q.size()) {
	foo temp = Q.front(); Q.pop();
	//	printf("%d %d %d\n", temp.x, temp.y, temp.d);
	for (int i = -1; i <= 1; i++)
	  for (int j = -1; j <= 1; j++) {
	    if (i && j) continue;
	    if (!i && !j) continue;

	    int tx = temp.x + i, ty = temp.y + j;
	    if (tx < 0 || ty < 0 || tx >= N || ty >= M) continue;
	    if (grid[tx][ty] == '.') continue;
	    if (d[tx][ty][1] >= maxd) continue;
	    maxd = d[tx][ty][1];

	    S.push_back(make_pair(tx,ty));
	    foo next = {tx,ty,temp.d+1};
	    Q.push(next);
	  }
      }

      memset(hit,0,sizeof(hit));
      for (int i = 0; i < S.size(); i++) {
	hit[S[i].first][S[i].second] = 1;
	ret += i+1;
      }

      for (int i = 0; i < N; i++)
	for (int j = 0; j < M; j++) {
	  if (grid[i][j] == '#' && !hit[i][j]) {
	    //	    printf("now %d %d = %d\n", i, j, min(d[i][j][0], d[i][j][1]));
	    ret += min(d[i][j][0], d[i][j][1]);
	  }
	}

      printf("Case #%d: %d\n", it, ret);
    }
  }
}
