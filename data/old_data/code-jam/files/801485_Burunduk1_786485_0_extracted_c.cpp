#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cctype>
#include <cmath>

#include <set>
#include <map>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

#ifdef _WIN32
#  define I64 "%I64d"
#else
#  define I64 "%Ld"
#endif

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define forab(i, a, b) for (int i = (int)(a); i <= (int)(b); i++)
#define forit(i, a) for (__typeof((a).begin()) i = (a).begin(); i != (a).end(); i++)
#define pb push_back
#define mp make_pair
#define zero(a) memset(a, 0, sizeof(a))
#define all(a) (a).begin(), (a).end()

#define IS(a, i) (((a) >> (i)) & 1)

typedef long long ll;
typedef vector <int> vi;

int Int( string s ) { int x; stringstream ss(s); ss >> x; return x; }
string Str( int x ) { stringstream ss; ss << x; return ss.str(); }

const int maxk = 21;

int k, C, win[maxk][maxk][maxk][maxk];

int Win( int i, int a, int b, int c )
{
  if (a < 0 || b < 0 || c < 0)
    return 0;
  if (i == k)
    return 1;
  
  int &res = win[i][a][b][c];
  if (res != -1)
    return res;
  res = 1;
  
//  printf("%d %d %d i=%d check\n", a, b, c, i);

  if (C >= 1 && !Win(i + 1, a - 1, b, c))
    res = 0;
  else if (C >= 2 && !Win(i + 1, a - 2, b, c) && !Win(i + 1, a, b - 1, c))
    res = 0;
  else if (C >= 3 && !Win(i + 1, a - 3, b, c) && !Win(i + 1, a - 1, b - 1, c) && !Win(i + 1, a, b, c - 1))
    res = 0;
    
//  if (res && i == 0)
//    printf("%d %d %d Win\n", a, b, c);
  return res;
}

int main() 
{
  int tn;
  scanf("%d", &tn);
  
  forab (tt, 1, tn)
  {
    scanf("%d%d", &k, &C);
	memset(win, -1, sizeof(win));

    int res = k * C;
	forn(x, k + 1)
	  forn(y, k + 1)
	    forn(z, k + 1)
		  if (x + y + z < res && Win(0, x, y, z))
		  {
//		    printf("%d %d %d\n", x, y, z);
		    res = x + y + z;
		  }
	printf("Case #%d: %d\n", tt, res);
  }
  return 0;
}
