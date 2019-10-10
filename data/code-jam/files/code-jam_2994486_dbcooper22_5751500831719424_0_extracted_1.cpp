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

string L[111];
int arr[111][111];

int main()
{
  freopen("input.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int TS;
  scanf("%d",&TS);
  FORE(ts,1,TS) {
    memset(arr,0,sizeof(arr));
    int N;
    scanf("%d",&N);
    REP(i,N) cin >> L[i];
    bool ok = 1;
    string dasar;
    REP(i,N) {
      string temp;
      REP(j,L[i].length()) {
        if(j == L[i].length()-1)
          temp.pb(L[i][j]);
        else if(L[i][j] != L[i][j+1]) temp.pb(L[i][j]);
      }
      if(i == 0) {
        dasar = temp;
      }
      else if(dasar != temp) {
        ok = 0;
        break;
      }
    }
    int len = dasar.length();
    if(ok)REP(i, N) {
      int now = 0;
      REP(j, len) {
        int cnt = 0;
        while(now < L[i].length() && L[i][now] == dasar[j]) {
          cnt++;
          now++;
        }
        arr[i][j] = cnt;
      }
    }
    int ans = 0;
    if(ok)
    REP(i, len) {
      int minn = NMAX;
      FORE(j,1, 100) {
        int tot = 0;
        REP(k, N) tot += abs(j - arr[k][i]);
        minn = min(minn, tot);
      }
      ans += minn;
    }
    if(!ok) printf("Case #%d: Fegla Won\n",ts);
    else printf("Case #%d: %d\n",ts,ans);
  }
  return 0;
}
