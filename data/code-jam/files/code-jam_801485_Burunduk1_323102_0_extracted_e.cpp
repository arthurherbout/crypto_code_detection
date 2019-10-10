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
#define sz(a) (int)(a).size()
#define pb push_back
#define mp make_pair
#define zero(a) memset(a, 0, sizeof(a))
#define all(a) (a).begin(), (a).end()

#define IS(a, i) (((a) >> (i)) & 1)

typedef long long ll;
typedef vector <int> vi;

int Int( string s ) { int x; stringstream ss(s); ss >> x; return x; }
string Str( int x ) { stringstream ss; ss << x; return ss.str(); }

typedef long double dbl;

const dbl eps = 1e-1;

struct pnt
{
  int x, y;

  pnt() { }
  pnt( int _x, int _y ) : x(_x), y(_y) { }
  
  pnt operator + ( pnt a ) { return pnt(x + a.x, y + a.y); } 
  pnt operator - ( pnt a ) { return pnt(x - a.x, y - a.y); } 

  int operator * ( pnt p ) { return x * p.y - y * p.x; } 
  int operator ^ ( pnt p ) { return x * p.x + y * p.y; } 
  
  int d2() { return x * x + y * y; }
  dbl d() { return sqrt((dbl)d2()); }
  
  pnt ort() { return pnt(-y, x); }
};

const int maxn = 1010;

int n, R, ans;
pnt p[maxn];

void Get( pnt p, dbl &d, dbl &a )
{
  d = p.d();
  a = atan2(p.y, p.x);
}

void Go( int last, int cur, int cnt, dbl maxR )
{
    //printf("last=%d cur=%d cnt=%d maxR=%.2lf\n", last, cur, cnt, (double)maxR);
	
	if (cnt > ans)
		ans = cnt;
	
	if (last == -1)
	{
	  forn(i, n) if (i)
	  {
	    dbl d, a;
		Get(p[i], d, a);
		
		dbl minR = maxR;
		int bad = 0;
		forn(j, n) if (j && i != j)
		{
			dbl dj, aj;
			Get(p[i], dj, aj);
			if (aj < a - eps && dj > d + eps)
				minR = min(minR, dj);
		}
		if (!bad && d < maxR - eps && minR - d > eps)
		  Go(cur, i, 1, minR - d);
	  }
	}
	else
	{
		while (1)
		{
			int ri = -1;
			dbl rAng = 0, rD = 0, tmpD;
			forn(i, n)
				if (((p[cur] - p[last]) * (p[i] - p[cur]) > 0 || 
						((p[cur] - p[last]) * (p[i] - p[cur]) == 0 &&
						((p[cur] - p[last]) ^ (p[i] - p[cur])) < 0)) &&
					(tmpD = (p[i] - p[cur]).d()) < maxR - eps)
				{
				  dbl ang = (p[cur] - p[last]) ^ (p[i] - p[cur]);
				  ang /= (p[cur] - p[last]).d();
				  ang /= (p[i] - p[cur]).d();
				  if (ri == -1 || (rAng < ang - eps) || (fabs(rAng - ang) < eps && rD < tmpD))
				    ri = i, rD = tmpD, rAng = ang;
				}
			if (ri == -1)
				break;
			//printf(": %d and %d\n", (p[cur] - p[last]) * (p[ri] - p[cur]), (p[cur] - p[last]) ^ (p[ri] - p[cur]));
			//printf("(%d,%d) ---> (%d,%d) ---> (%d,%d)\n", p[last].x, p[last].y, p[cur].x, p[cur].y, p[ri].x, p[ri].y);
			Go(cur, ri, cnt + 1, maxR - rD);
			maxR = rD;
		}
	}
}

int main() 
{
  int tn;
  scanf("%d", &tn);
  
  forab (tt, 1, tn)
  {
    cin >> n >> R;
	forn(i, n)
	  cin >> p[i].x >> p[i].y;
	assert(p[0].x == 0 && p[0].y == 0);
	ans = 0;
	Go(-1, 0, 0, R);
	printf("Case #%d: %d\n", tt, ans);
  }
  return 0;
}
