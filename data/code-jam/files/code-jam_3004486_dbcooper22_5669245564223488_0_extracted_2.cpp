#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <list>
#include <cstring>
#include <stack>
#include <bitset>

using namespace std;

#define NMAX 2147483647
#define LMAX 9223372036854775807LL
#define pb push_back
#define pob pop_back
#define mp make_pair
#define st first
#define nd second
#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i,a,b) for(int i=(a);i>(b);--i)
#define REP(i,n) FOR(i,0,n)
#define FORE(i,a,b) for(int i=(a);i<=(b);++i)

template<class T> T gcd(T a,T b) { return (b==0)?a:gcd(b,a%b); }
template<class T> T lcm(T a,T b) { return a*(b/gcd(a,b)); }
int DAY[] = {31, 28, 31, 30, 31 , 30, 31, 31, 30 , 31, 30, 31};
string Rs[] = {"I","IV","V","IX","X","XL","L","XC","C","CD","D","CM","M"};
int Ri[]   = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
double eps = 1e-9;

bool pos(string S) {
  bool f[26];
  int len = S.length();
  memset(f,0,sizeof(f));
  REP(i,len) S[i] -= 'a';
  REP(i,len) {
    if(i == len - 1) {
      if(f[S[i]]) return 0;
    }
    else if(S[i] == S[i+1]) continue;
    else {
      if(f[S[i]]) return 0;
      f[S[i]] = 1;
    }
  }
  return 1;
}

bool f[11][26];
string S[11];
int L[11];
int TS, N;

bool poss() {
  bool g6[26];
  memset(g6,0,sizeof(g6));
  REP(i,26) g6[i] = f[L[0]][i];
  FOR(i,1,N) {
    int len1 = S[L[i-1]].length();
    int a = S[L[i-1]][len1-1];
    int len2 = S[L[i]].length();
    int b = S[L[i]][0];
    bool ok = (a == b);
    REP(j,26) {
      if(ok && j == a) continue;
      if(g6[j] == 1 &&  f[L[i]][j] == 1)  {
          return 0;
      }
      else g6[j] |= f[L[i]][j];
    }
  }
  return 1;
}

int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  cin >> TS;
  FORE(ts,1,TS) {
    bool ok = 1;
    memset(f,0,sizeof(f));
    cin >> N;
    REP(i, N) cin >> S[i];
    REP(i,N)REP(j,S[i].length()) S[i][j] -= 'a';
    REP(i, N) {
      REP(j, S[i].length()) {
        int c = S[i][j];
        if(j == S[i].length() - 1) {
          if(f[i][c]) ok = 0;
          f[i][c] = 1;
        }
        else if(S[i][j] == S[i][j+1]) continue;
        else {
          if(f[i][c]) ok = 0;
          f[i][c] = 1;
        }
      }
    }
    if(!ok) {
      printf("Case #%d: 0\n",ts);
      continue;
    }
    int ans = 0;
    REP(i, N) L[i] = i;
    do {
      if(poss()) ans++;
    }while(next_permutation(L, L + N));
    printf("Case #%d: %d\n",ts,ans);
  }
  return 0;
}
