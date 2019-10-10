#include <cstdio>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

double _a(int a, int b, int c, int d, int e, int f) {
  double A = hypot(c-a, d-b);
  double B = hypot(e-c, f-d);
  double C = hypot(e-a, f-b);
  double S = (A+B+C)/2;
  return sqrt(S*(S-A)*(S-B)*(S-C));
}

int N, M, A;
int main() {
  int C; scanf("%d", &C);
  for (int it = 1; it <= C; it++) {
    scanf("%d %d %d", &N, &M, &A);

    printf("Case #%d: ", it);

    for (int x = 0; x <= N; x++)
      for (int w = 0; w <= M; w++)
	for (int z = x; z <= N; z++)
	  for (int y = w; y <= M; y++) {
	    int v = z*y - x*w;
	    if (v == A) {
	      printf("%d %d %d %d %d %d\n", 0, 0, x, y, z, w);
	      goto done;
	    }
	  }

    printf("IMPOSSIBLE\n");
  done:;
  }
}
