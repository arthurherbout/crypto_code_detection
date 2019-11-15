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

int main() {
  int tests;
  cin >> tests;
  for(int tt = 1; tt <= tests; tt++) {
    int trials, numq;
    cin >> trials >> numq;
    double p[numq][4];
    FOR(i, numq) {
      FOR(j, 4) {
	cin >> p[i][j];
      }
    }
    vector< ld > probs;
    for(int mask = 0; mask < (1 << numq) * (1 << numq); mask++) {
      int cur = mask;
      double pp = 1.0000;
      FOR(i, numq) {
	pp *= p[i][cur % 4];
	cur = cur / 4;
      }
      probs.pb(-pp);
    }
    sort(probs.begin(), probs.end());
    ld sum = 0;
    if(probs.size() <= trials) sum = -1.00;
    else {
      sum = 0.000;
      FOR(i, trials) {
	sum += (probs[i]);
      }
      //      sum = 1.00 - sum;
      //      sum = -sum;
    }
    cout << "Case #" << tt  << ": " << -sum << endl;
  }
  return 0;
}

