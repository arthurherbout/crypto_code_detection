#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib> 
using namespace std;
 
#define sqr(a) ((a)*(a)) 
#define FOR(i,n) for (long (i)=0; (i)<(n); (i)++) 
#define SET(a,k) memset((a),(k),sizeof((a)))
 
typedef long long ll;

const long MaxN = 1005;
const long MaxSet = 1 << 11;

const ll MOD = 30031;

ll dp[MaxSet][MaxN];
bool flag[MaxSet][MaxN];

long n, k, p; 

FILE *fin, *fout;

ll memoiz(long s, long i)
{  

  if (i == n)
  {
    bool ok = true;
    for (int j = 0; j < k; j++)
      if (!(s & (1 << j)))
      {
        ok = false;
        break;
      }      
    if (ok) return 1;
    else return 0;
  }
  
  else if (flag[s][i]) return dp[s][i];
  
  else
  {
    ll suma = 0;
    if (s & (1<<(p-1)))
    {      
      long sp = s ^ (1<<(p-1));
      sp <<= 1;
      sp |= 1;
      suma += memoiz(sp, i + 1);
    }
    else
    {
      for (int j = 0; j < p; j++)
      {
        if (!(s & (1 << j))) continue;
         
        long sp = s ^ (1<<j);
        sp <<= 1;
        sp |= 1;
        
        suma += memoiz(sp, i + 1);
        suma %= MOD;
      }
    }
    
    flag[s][i] = true;
    dp[s][i] = suma;
    
    return suma;
  }       
}

void solve(int test)
{
  SET(flag, 0);
  fscanf(fin,"%d %d %d", &n, &k, &p);
  
  long s = 0;
  for (int i = 0; i < k; i++)
  {
    s <<= 1;
    s |= 1;
  }
  fprintf(fout, "Case #%d: %ld\n", test, memoiz(s, k));
}

int main()
{
  fin = fopen("D-smallin", "r");
  fout = fopen("D_small_aa.out", "w");
  
  int test;
  fscanf(fin, "%d", &test);
  for (int t = 1; t <= test; t++)
  {     
    solve(t);
  }
  
  fclose(fin);
  fclose(fout);
  
  return 0;
}
