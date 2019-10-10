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

int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int TS;
  scanf("%d",&TS);
  FORE(ts,1,TS) {
    int A, B, K, ans = 0;
    scanf("%d%d%d",&A,&B,&K);
    REP(i,A)REP(j,B) {
      int temp = (i & j);
      if(temp < K) ans++;
    }
    printf("Case #%d: %d\n",ts,ans);
  }
  return 0;
}
