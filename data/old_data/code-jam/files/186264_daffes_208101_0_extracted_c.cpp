#include <iostream>
#include <map>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>
#include <string>

using namespace std;

int MAXP;

char rest[1000], res[1000];
int mina;
char tab[30][30];
int n;
int query;
int p;

#define ok(y,x) (y >= 0 && y < n && x >= 0 && x < n)

int dx[] = { -2, -1, -1, -1, -1, 0, 0, +1, +1, +1, +1, +2, 0, 0, 0, 0 };
int dy[] = { 0, 1, 1, -1, -1, +2, -2, +1, +1, -1, -1, 0, 0, 0, 0, 0 };
int sx[] = { -1, -1, 0, -1, 0, 0, 0, 1, 0, 1, 0, 2, +1, -1, 0, 0};
int sy[] = { 0, 0,   1, 0, -1, +2, -2, 0, +1, 0, -1, 0,  0, 0, +1, -1};

#define CORR 10000

int pd[11][11][CORR*10];

#define INF 0x3f3f3f3f

void dfs(int i, int j, int val, int dep) {
  rest[p++] = tab[i][j] + '0';
  if (pd[i][j][val + CORR] != INF  && pd[i][j][val + CORR] <= dep) {
    p--;
    return;
  }
  pd[i][j][val + CORR] = dep;
  if (dep > mina || dep > MAXP) {
    p--;
    return;
  }
  if (val == query) {
    //cout << res << endl;
    rest[p] = '\0';
    if (dep < mina) {
      strcpy(res,rest);
      mina = dep;
    }
    if (dep == mina && strcmp(rest,res) < 0) {
      strcpy(res,rest);
    }
    p--;
    return;
  }
  int val2;
  for (int k = 0; k < 16; k++) {
    int ii = i + dy[k];
    int jj = j + dx[k];
    int si = i + sy[k];
    int sj = j + sx[k];
    if (!ok(ii,jj) || !ok(si,sj)) continue;
    if (tab[si][sj] == '+') {
      val2 = val + tab[ii][jj];
      rest[p++] = '+';
    }
    else {
      val2 = val - tab[ii][jj];
      rest[p++] = '-';
    }
    dfs(ii,jj,val2,dep+1);
    p--;
  }
  p--;
}

int main() {
  int k;
  int test = 0;
  scanf("%d",&k);
  while (k--) {
    test++;
    printf("Case #%d:\n",test);
    int nq;
    scanf("%d %d",&n,&nq);
    for (int i = 0; i < n; i++) {
      scanf("%s",tab[i]);
      for (int j = 0; j < n; j++) {
	if (tab[i][j] >= '0' && tab[i][j] <= '9') tab[i][j] -= '0';
      }
    }
    for (int ii = 0; ii < nq; ii++) {
      scanf("%d",&query);
      MAXP = 15;
    denovo:
      mina = INF;
      p = 0;
      memset(pd,INF,sizeof(pd));
      for (int i = 0; i < n; i++) {
	for (int j = 0; j < n; j++) {
	  if (tab[i][j] != '+' && tab[i][j] != '-') {
	    dfs(i,j,tab[i][j],0);
	  }
	}
      }
      if (mina == INF) {
	MAXP += 10;
	goto denovo;
      }
      printf("%s\n",res);
    }
  }
  return 0;
}
 
