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

map <long long, bool> mapa;

int f(long long a, long long b) {
  int ans = 0;
  long long g = gcd(a, b);
  a /= g;
  b /= g;
  long long temp = b;
  while(temp % 2 == 0) {
    temp /= 2LL;
    ans++;
  }
  if(temp != 1) return -1;
  if(a == 1) return ans;
  if(a > b / 2) return 1;
  return min(ans, 1 + f(a - 1LL, b/2LL));
}

int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  long long p = 1;
  int TS;
  scanf("%d",&TS);
  FORE(ts,1,TS) {
    long long a, b;
    scanf("%I64d/%I64d",&a,&b);
    int ans = f(a, b);
    if(ans == -1) {
      printf("Case #%d: impossible\n",ts);
    }
    else {
      printf("Case #%d: %d\n",ts, ans);
    }
  }
  return 0;
}
