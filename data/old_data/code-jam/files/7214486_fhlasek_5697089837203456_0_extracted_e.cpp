/* Writen by Filip Hlasek 2014 */
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>

#define FOR(i,a,b) for(int i=(a);i<=(b);i++)
#define FORD(i,a,b) for(int i=(a);i>=(b);i--)
#define REP(i,b) for(int i=0;i<(b);i++)

using namespace std;

int A, B;

#define MAX_VAL 111111
long long end[MAX_VAL];

int main(int argc, char *argv[]) {
  int T;
  scanf("%d", &T);
  FOR(testcase, 1, T) {
    long long N;
    scanf("%lld%d%d", &N, &A, &B);
    printf("Case #%d: ", testcase);
    REP(i, MAX_VAL) end[i] = -1;
    end[0] = 1;

    FOR(val, 0, MAX_VAL - 1) if (end[val] != -1) {
      if (end[val] >= N) { printf("%d\n", val); break; }
      FOR(i, 0, val) if (end[i] != -1) {
        int nval = max(i + B, val + A);
        if (end[nval] == -1 || end[val] + end[i] > end[nval]) end[nval] = end[val] + end[i];
      }
    }
  }
 return 0;
}
