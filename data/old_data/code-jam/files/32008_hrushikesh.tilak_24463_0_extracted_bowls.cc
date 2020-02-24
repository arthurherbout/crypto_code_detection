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

map<string, int> code;
int mix;
vi edge[2000];
int num;

void add(string ss) {
  if(code.count(ss)) return;
  if(ss[0] <= 'Z') {
    code[ss] = num;
  }
  else {
    code[ss] = -num;
  }
  num++;
}

int find(int v) {
  int ret = 0;
  if(v < 0);
  else {
    vi all;
    FOR(i, s(edge[v])) {
      all.pb(find(edge[v][i]));
    }
    sort(all.begin(), all.end());
    int mx = all[all.size() - 1];
    if(mx == 0) return 1;
    if(mx == 1) {
      ret = 1;
      FOR(i, s(all)) ret += all[i];
    }
    else {
      int sall = all.size();
      if(sall == 1) ret = mx;
      else {
	if(all[sall-2] < mx) ret = mx;
	else ret = mx + 1;
      }
    }
  }
  cerr << v << ' ' << ret << endl;
  return ret;
}
  
  
int main() {
  int tests;
  cin >> tests;
  for(int tt = 1; tt <= tests; tt++) {
    cin >> mix;
    code.clear();
    num = 0;
    FOR(i, 2000) edge[i].clear();
    while(mix--) {
      string what;
      int howmany;
      cin >> what;
      add(what);
      cerr << what << ' ' << code[what] << endl;
      cin >> howmany;
      while(howmany--) {
	string ing;
	cin >> ing;
	add(ing);
	cerr << ing << ' ' << code[ing] << endl;
	edge[code[what]].pb(code[ing]);
      }
    }

    cout << "Case #" << tt << ": " << find(0) << endl;
  }
  return 0;
}
    
      
      
      
    
    
    

