#include <cstdio>
#include <algorithm>
using namespace std;

char buf[50];
int r[50];
int main() {
  int T; scanf("%d", &T);
  for (int it = 1; it <= T; it++) {
    int N; scanf("%d", &N);
    for (int i = 0; i < N; i++) {
      scanf("%s", buf);
      r[i] = -1;
      for (int j = N-1; j >= 0; j--) {
	if (buf[j] == '1') {
	  r[i] = j;
	  break;
	}
      }
    }

    // now arrange things so that r[i] <= i for all i

    int ret = 0;
    for (int t = 0; t < N; t++) {
      for (int s = t; s < N; s++) {
	if (r[s] <= t) {
	  for (int i = s; i > t; i--) {
	    swap(r[i], r[i-1]);
	    ret++;
	  }
	  break;
	  
	}
      }
    }
    printf("Case #%d: %d\n", it, ret);
  }
}

