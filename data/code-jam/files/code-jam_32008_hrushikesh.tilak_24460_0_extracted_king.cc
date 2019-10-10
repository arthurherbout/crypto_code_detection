#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

int _a;

#define FOR(i , n) for(int i = 0 ; i < n ; i++)
#define FOT(i , a , b) for(int i = a ; i < b ; i++)
#define GETINT (scanf("%d" , &_a) , _a)
#define pb push_back
#define mp make_pair
#define s(a) (int(a.size()))
#define PRINT(a) cerr << #a << " = " << a << endl

typedef long long ll;
typedef long double ld;
typedef vector<int> vi;
typedef vector<string> vs;
typedef pair<int , int> PII;
typedef vector< PII > vpii;

map< vs, bool > win;

string parse(bool what) {
  if(what) return "A";
  else return "B";
}

int movex[] = {0,0,1,1,1,-1,-1,-1};
int movey[] = {1,-1,-1,0,1,-1,0,1};

bool find(vs cfg) {
  if(win.count(cfg)) return win[cfg];
  int kr, kc;
  win[cfg] = false;
  bool& ans = win[cfg];
  FOR(i, cfg.size()) FOR(j, cfg[0].size()) if(cfg[i][j] == 'K') {
    kr = i;
    kc = j;
    i = 100000;
    break;
  }
  FOR(m, 8) {
    int nr = kr + movex[m];
    int nc = kc + movey[m];
    if(nr >= 0 && nc >= 0 && nr < cfg.size() && nc < cfg[0].size() &&
       cfg[nr][nc] != '#') {
      vs nu = cfg;
      nu[kr][kc] = '#';
      nu[nr][nc] = 'K';
      win[cfg] = win[cfg] || !find(nu);
    }
  }
  cerr << "-----" << endl;
  FOR(i, cfg.size()) cerr << cfg[i] << endl;
  cerr << ans << endl;
  cerr << "-----" << endl;
  return ans;
}
  


int main() {
  int tests;
  cin >> tests;
  for(int tt = 1; tt <= tests; tt++) {
    vs cfg;
    int R, C;
    cin >> R >> C;
    FOR(i, R) {
      string t;
      cin >> t;
      cfg.pb(t);
    }
    cout << "Case #" << tt  << ": " << parse(find(cfg)) << endl;
  }
  return 0;
}


