#include <cstdio>
#include <cstdlib>
using namespace std;

int H, W;
int alt[101][101];

int c = 0;
int p[10001];
int s[50];
int dx[] = {-1,0,0,1};
int dy[] = {0,-1,1,0};

int root(int x) {
  if (p[x] == -1) return x;
  return p[x] = root(p[x]);
}
void merge(int x, int y) {
  int X = root(x), Y = root(y);
  if (X == Y) return;
  if (rand() & 1) p[X] = Y;
  else p[Y] = X;
}

int main() {
  int N; scanf("%d", &N);
  int cn = 1;
  while (N--) {
    c = 0;
    scanf("%d %d", &H, &W);
    for (int i = 0; i < H; i++)
      for (int j = 0; j < W; j++)
	scanf("%d", alt[i]+j);

    printf("Case #%d:\n", cn++);

    for (int i = 0; i < H; i++)
      for (int j = 0; j < W; j++)
	p[i*W+j] = -1;

    for (int i = 0; i < H; i++)
      for (int j = 0; j < W; j++) {
	int lowest = alt[i][j];
	int dir = -1;
	for (int k = 0; k < 4; k++) {
	  int x = i + dx[k], y = j + dy[k];
	  if (x < 0 || y < 0 || x >= H || y >= W)
	      continue;
	  if (alt[x][y] < lowest)
	    lowest = alt[x][y], dir = k;
	}

	if (dir != -1) {
	  int x = i + dx[dir], y = j + dy[dir];
	  merge(i*W+j, x*W+y);
	}
      }
    for (int i = 0; i < H; i++) {
      for (int j = 0; j < W; j++) {
	if (j > 0) printf(" ");
	int R = root(i*W+j);
	int idx = -1;
	bool f = 0;
	for (int k = 0; k < c; k++) {
	  if (s[k] == R) {
	    idx = k;
	    f = 1;
	    break;
	  }
	}
	if (!f) {
	  idx = c;
	  s[c] = R;
	  c++;
	}
	printf("%c", idx+'a');
      }
      printf("\n");
    }
  }
}
