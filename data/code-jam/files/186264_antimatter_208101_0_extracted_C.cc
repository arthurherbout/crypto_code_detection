#include <cstdio>
#include <string>
#include <queue>
using namespace std;

char sq[25][25];
string best[21][21][291]; // may need to jump below zero!
int W;

struct state {
  int x, y, v;
  string str;
};
state make(int a, int b, int c, string d) {state ret={a,b,c,d}; return ret;}
int dx[] = {1,0,-1,0};
int dy[] = {0,1,0,-1};

bool better(const string &a, const string &b) {
  if (a == "") return false;
  if (b == "") return true;
  if (a.size() < b.size()) return true;
  if (a.size() > b.size()) return false;
  if (a < b) return true;
  return false;
}

void calc() {
  for (int i = 0; i < W; i++)
    for (int j = 0; j < W; j++)
      for (int k = 0; k < 291; k++) best[i][j][k] = "";

  queue<state> Q;
  for (int i = 0; i < W; i++)
    for (int j = 0; j < W; j++)
      if (sq[i][j] != '-' && sq[i][j] != '+') {
	int n = sq[i][j]-'0';
	string z = ""; z += sq[i][j];
	Q.push(make(i,j,n,z));
	best[i][j][n+20] = z;
      }

  while (Q.size()) {
    state R = Q.front(); Q.pop();

    if (best[R.x][R.y][R.v+20] != R.str) continue;

    for (int i = 0; i < 4; i++) {
      int tx = R.x+dx[i], ty = R.y+dy[i];
      if (tx < 0 || ty < 0 || tx >= W || ty >= W) continue;
      for (int j = 0; j < 4; j++) {
	int ttx = tx+dx[j], tty = ty+dy[j];
	if (ttx < 0 || tty < 0 || ttx >= W || tty >= W) continue;
	
	char op = sq[tx][ty];
	int n = sq[ttx][tty] - '0';
	int nv = R.v;

	if (op == '+') nv += n;
	else nv -= n;

	if (nv + 20 < 0 || nv + 20 >= 290) continue;

	string k = R.str;
	k += op;
	k += ('0'+n);
	if (better(k, best[ttx][tty][nv+20])) {
	  best[ttx][tty][nv+20] = k;
	  Q.push(make(ttx,tty,nv,k));
	}
      }
    }
  }
}

int main() {
  int T; scanf("%d", &T);
  for (int it = 1; it <= T; it++) {
    printf("Case #%d:\n", it);
    int Q;
    scanf("%d %d", &W, &Q);
    for (int i = 0; i < W; i++) scanf("%s", sq[i]);
    
    calc();

    for (int i = 0; i < Q; i++) {
      int x; scanf("%d", &x);
      x += 20;
      string b = "";
      for (int j = 0; j < W; j++)
	for (int k = 0; k < W; k++) {
	  if (better(best[j][k][x], b))
	    b = best[j][k][x];
	}

      printf("%s\n", b.c_str());
    }
  }
}
