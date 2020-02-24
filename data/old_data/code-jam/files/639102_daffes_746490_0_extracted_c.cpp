#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <map>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
#include <numeric>
#include <limits.h>

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int,int> ii;
typedef long long ll;

#define ITE(v) typeof(v.begin())
#define FOR(i,n) for(int i = 0; i < n; i++)
#define FORIT(it,v) for(ITE(v) it = v.begin(); it != v.end(); it++)
#define ALL(v) v.begin(), v.end()
#define SZ(v) int(v.size())
#define pb push_back
#define SQR(a) ((a)*(a))

#define INF 0x3f3f3f3f
#define EPS (1e-8)

inline int cmp(double a, double b = 0.0) {
  if (fabs(a-b) <= EPS) return 0;
  if (a < b) return -1;
  return 1;
}



map<int,int> s;

int main() {
  int ncases;
  scanf("%d",&ncases);
  FOR(kk,ncases) {
    printf("Case #%d: ",kk+1);
    int nc,p,v;
    s.clear();
    scanf("%d",&nc);
    FOR(i,nc) {
      scanf("%d %d",&p,&v);
      s[p] += v;
    }
    int res = 0;
    stack<map<int,int>::iterator> st;
    bool stop = false;
    while (s.size() && !stop) {
      stop = true;
      FORIT(it,s) {
        int k = it->second/2;
        if (!k) continue;
        stop = false;
        res += k;
        s[it->first-1] += k;
        s[it->first+1] += k;
        it->second %= 2;
        if (it->second == 0) st.push(it);
      }
      while (!st.empty()) {
        if (st.top()->second == 0) s.erase(st.top());
        st.pop();
      }
    }
    printf("%d\n",res);
  }  
  return 0;
}
