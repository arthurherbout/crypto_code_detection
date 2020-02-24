#include <cstdio>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

#define INF 1234

int N;
int now[2005];
char op[20];
int opid[20];
int best;
set<int> known;

void skus(int i) {
  if (i == N) {
    int cur = now[0];
    for (set<int>::iterator it = known.begin(); it != known.end(); ++it) {
      if (now[*it] == 1) cur++;
    }
    best = min(best, cur);
    return;
  }

  int id = opid[i];

  if (op[i] == 'E') {
    if (id) {
      if (now[id] == 1) return;
      int old = now[id];
      now[id] = 1;
      skus(i+1);
      now[id] = old;
    } else {
      now[0]++;
      skus(i+1);
      now[0]--;
      for (set<int>::iterator it = known.begin(); it != known.end(); ++it) {
        if (now[*it] == 1) continue;
        int old = now[*it];
        now[*it] = 1;
        skus(i+1);
        now[*it] = old;
      }
    }
  } else {
    if (id) {
      if (now[id] == 0) return;
      int old = now[id];
      now[id] = 0;
      skus(i+1);
      now[id] = old;
    } else {
      if (now[0]) {
        now[0]--;
        skus(i+1);
        now[0]++;
      } else {
        skus(i+1);
      }
      for (set<int>::iterator it = known.begin(); it != known.end(); ++it) {
        if (now[*it] == 0) continue;
        now[*it]--;
        skus(i+1);
        now[*it]++;
      }
    }
  }
}

int main() {
  int T;
  scanf("%d", &T);
  for (int tc = 1; tc <= T; tc++) {
    for (int i = 0; i < 2005; i++) now[i] = -1;
    now[0] = 0;
    known.clear();

    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
      scanf(" %c %d", &op[i], &opid[i]);
      if (opid[i]) known.insert(opid[i]);
    }

    best = INF;

    skus(0);

    printf("Case #%d: ", tc);
    if (best == INF) printf("CRIME TIME\n");
    else printf("%d\n", best);
    fflush(stdout);
  }
}
