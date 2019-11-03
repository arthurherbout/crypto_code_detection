#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
using namespace std;

#define all(c) ((c).begin()), ((c).end()) 
#define iter(c) __typeof((c).begin())
#define present(c, e) ((c).find((e)) != (c).end()) 
#define cpresent(c, e) (find(all(c), (e)) != (c).end())
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define tr(c, i) for (iter(c) i = (c).begin(); i != (c).end(); ++i)
#define pb(e) push_back(e)
#define mp(a, b) make_pair(a, b)

typedef long long ll;

struct person { int A, B, C; };

struct cmpA {
  bool operator () (const person &a, const person &b) const {
    return a.A < b.A;
  }
};

struct cmpB {
  bool operator () (const person &a, const person &b) const {
    return a.B < b.B;
  }
};


person P[5010];

void solve() {
  int N;
  scanf("%d", &N);

  rep (i, N) scanf("%d%d%d", &P[i].A, &P[i].B, &P[i].C);
  sort(P, P + N, cmpA());

  int ans = 0;

  //puts("");
  rep (i, N) {
    // i made ireru

    int a = P[i].A;
    //printf("%d\n", i);
    
    sort(P, P + i + 1, cmpB());
    //vector<int> cval;
    priority_queue<int> cque;
    rep (j, i + 1) {
      int b = P[j].B;
      int c = 10000 - a - b;

      //cval.pb(P[j].C);
      //sort(all(cval));
      cque.push(P[j].C);
      while (!cque.empty() && cque.top() > c) cque.pop();
      

      //printf("abc: %d %d %d\n", a, b, c);
      //rep (k, cval.size()) printf(" %d", cval[k]); 
      
      //int num = upper_bound(all(cval), c) - cval.begin();
      int num = cque.size();
      //printf(" --- num: %d\n", num);
      ans = max(num, ans);
    }
  }

  printf("%d\n", ans);
}


int main() {
  int cases;
  scanf("%d", &cases);
  rep (ca, cases) {
    printf("Case #%d: ", ca + 1);
    solve();
  }
  return 0;
}

