#include <sstream>
#include <iostream>
#include <cstdio>
#include <string.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <deque>

using namespace std;

typedef long long ll;
typedef vector<int> VI;
typedef pair<int, int> PI;

#define foru(i,a,b) for(int i = int(a); i <= int(b); ++i)
#define ford(i,b,a) for(int i = int(b); i >= int(a); --i)
#define rep(i, n) for(int i = 0; i < int(n); ++i)
#define all(a) a.begin(),a.end()
#define size(a) int(a.size())
#define fill(a,x) memset(a, (x), sizeof(a))
#define mp(x,y) make_pair((x), (y))
#define pb(x) push_back((x))
#define fr first
#define sc second

const int MAX_N = 30;
const int MAX_S = 2000007;

int n;
int a[MAX_N];
VI b[MAX_S];


bool check(int x, int y) {
   rep(i, n)
      if (x&(1<<i) == 1 && y&(1<<i) == 1) return false;
   return true;
}

void print_res(int x) {
   rep(i, n)
      if ((x>>i)&1) printf("%d ",a[i]);
   printf("\n");
}

int main() {
   freopen("C0.in", "r", stdin);
   freopen("C.out", "w", stdout);
   int nTest;
   cin >> nTest;
   foru(testid, 1, nTest) {
      scanf("%d", &n);
      rep(i, n) scanf("%d", &a[i]);
      rep(i, MAX_S+1) b[i].clear();
      foru(i, 0, (1<<n)-1) {
         int s = 0;
         rep(j, n)
            if ((i>>j)&1) s += a[j];
         b[s].pb(i);
         //cout << " " << i << " " << s << endl;
      }

      int r1 = -1, r2 = -1;
      foru(i, 1, MAX_S)
         if (r1 > 0) break;
         else
            rep(j, size(b[i]))
               foru(k, j+1, size(b[i])-1)
                  if (check(b[i][j], b[i][k])) {
                     r1 = b[i][j];
                     r2 = b[i][k];
                     break;
                  }
      printf("Case #%d:\n", testid);
      if (r1 < 0) printf("Impossible\n");
      else {
         print_res(r1);
         print_res(r2);
      }
   }
   return 0;
}
