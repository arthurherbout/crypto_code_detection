#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;

int N;
int plants[50][3];
int main() {
  int C; scanf("%d", &C);
  for (int it = 1; it <= C; it++) {
    scanf("%d", &N);
    for (int i = 0; i < N; i++) scanf("%d %d %d", plants[i], plants[i]+1, plants[i]+2);

    double ret = 1e6;
    if (N == 1)
      ret = plants[0][2];
    else if (N == 2)
      ret = max(plants[0][2], plants[1][2]);
    else { // N == 3
      for (int i = 0; i < 3; i++)
	for (int j = i+1; j < 3; j++) {
	  double one = plants[3 - i - j][2];
	  double two = 0.5 * (hypot(plants[i][0]-plants[j][0], plants[i][1]-plants[j][1]) + plants[i][2] + plants[j][2]);
	  ret = min(ret, max(one, two));
	}
    }
    printf("Case #%d: %.6lf\n", it, ret);
  }
}
