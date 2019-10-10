//DEDICATED TO EMMA WATSON, THE BRITISH *SUNSHINE*
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
//#include <fstream>

#define eps 10e-10
#define INF 1000000000
#define PI 3.141592653589793238462
#define EU 2.71828182845904523536
#define sz(a) (int)a.size()
#define pb(a) push_back(a)
#define mset(a,hodnota) memset(a,hodnota,sizeof(a))
#define wh(a) a.begin(),a.end()
#define REP(i,n) for(__typeof(n) i=0;i<(n);++i)
#define REPS(i,n) for(int(i)=0;i<int(n.size());++i)
#define FOR(i,a,b) for(__typeof(b) i=(a);i<=(b);++i)
#define FORD(i,a,b) for(__typeof(a) i=(a);i>=(b);--i)
#define FORE(it,c) for(__typeof((c).begin()) it=(c).begin();it!=(c).end();++it)

#define SQR(a) ((a)*(a))
#define pii pair<int,int>
#define mp(a,b) make_pair(a,b)
#define fi first
#define se second
typedef long long ll;
typedef long double ld;
using namespace std;
#define MID 210
#define H 420
int N;
string s;int x;
int M[MID*2][2*MID];
int dir[4][2]={1,0, 0,1, -1,0, 0,-1};
inline bool check(int r,int c)
{
  //boundary
  if (M[r][c])
    return 1;

  //is outside
  int poc=0,cr=r;
  while(cr>=0)
  {
    if (M[cr][c])
      poc++;
    cr--;
  }
  if (poc%2==1) return 0;
  //0-2
  int nr1=r,nr2=r;
  while(nr1<H&&!M[nr1][c])nr1++;
  while(nr2>=0&&!M[nr2][c])nr2--;
  if (nr1<H&&nr2>0) return 1;
  //1-3
  int nc1=c,nc2=c;
  while(nc1<H&&!M[r][nc1])nc1++;
  while(nc2>=0&&!M[r][nc2])nc2--;
  if (nc1<H&&nc2>0) return 1;
  return 0;
}
void solve_case()
{
  mset(M,0);
  scanf("%d",&N);
  int R=MID,C=MID,D=0;
  REP(i,N)
  {
    cin>>s>>x;
    REP(j,x)    
      REPS(k,s)
      {
        int oldR=R,oldC=C;
        if (s[k]=='F')R+=dir[D][0]*2,C+=dir[D][1]*2;
        else if (s[k]=='R')D=(D+1)%4;
        else D=(D+3)%4;
        if (oldR!=R)        
          for(int r=min(oldR,R);r<=max(oldR,R);r++) M[r][C]=1;
        if (oldC!=C)        
          for(int c=min(oldC,C);c<=max(oldC,C);c++) M[R][c]=1;        
        
      }    
  }
      
  int ans=0;
  REP(r,H)
    REP(c,H)
    if (r%2==1&&c%2==1)
    {
      ans+=check(r,c);
    }     
  printf("%d\n",ans);
}


int cases;
int main( )
{
  scanf("%d\n",&cases);
  REP(ii,cases)
  {
    printf("Case #%d: ",ii+1);
    solve_case();
  }         
  return 0;
}
