#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define bublic public
#define clr(x) memset((x), 0, sizeof(x))
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define mp make_pair
#define sz size()
#define For(i, st, en) for(int i=(st); i<=(int)(en); i++)
#define Ford(i, st, en) for(int i=(st); i>=(int)(en); i--)
#define forn(i, n) for(int i=0; i<(int)(n); i++)
#define ford(i, n) for(int i=(n)-1; i>=0; i--)
#define fori(it, x) for (__typeof((x).begin()) it = (x).begin(); it != (x).end(); it++)

template <class _T> inline _T sqr(const _T& x) { return x * x; }
template <class _T> inline string tostr(const _T& a) { ostringstream os(""); os << a; return os.str(); }
template <class _T> inline istream& operator << (istream& is, const _T& a) { is.putback(a); return is; }

typedef long double ld;

// Constants
const ld PI = 3.1415926535897932384626433832795;
const ld EPS = 1e-11;

// Types
typedef signed   long long i64;
typedef unsigned long long u64;
typedef set < int > SI;
typedef vector < ld > VD;
typedef vector < int > VI;
typedef vector < bool > VB;
typedef vector < string > VS;
typedef map < string, int > MSI;
typedef pair < int, int > PII;

const int INF = 0x2f3f3f3f;

int qq;

/////////////////////////////////
#define MAX_N 2*(1010)
long long X, t;
int S, R, N;
long long B[MAX_N], E[MAX_N];
long long intervals[MAX_N];
int W[MAX_N];

void swap( int W[], long long intervals[], int x, int y )
{
	long interval = intervals[x];
	int wtemp = W[x];
	
	intervals[x] = intervals[y];
	W[x] = W[y];

	intervals[y] = interval;
	W[y] = wtemp;
}

void sort( long long intervals[] , int W[], int N )
{
	forn ( i, N )
	{
		For(j, i+1, N-1 )
		{
			if ( W[j] < W[i] )
			{
				swap( W, intervals, i, j );
			}
		}
	}
}

double f()
{
	sort( intervals, W, N );
	double totalTime= 0;

	forn ( i, N )
	{		
		if (totalTime<t )
		{
			double thisDist = intervals[i];
			double thisRunTime = thisDist/(double)(R+W[i]);
			if ( totalTime + thisRunTime <= t )
			{
				totalTime += thisRunTime;
				continue;
			}
			else
			{
				double timeToRun = t - totalTime;
				double dist = timeToRun*(R+W[i]);
				double distLeft = intervals[i] - dist;
				totalTime = t + ((double)distLeft)/(double)(S+W[i]);
			}
		}
		else
		{
			double thisRunTime = ((double)intervals[i])/(double)(S+W[i]);
			totalTime += thisRunTime;
		}
	}
	return totalTime;
}

int main()
{
	freopen("A-large.in", "rt", stdin);
	freopen("A-large.out", "wt", stdout);
	cout << setiosflags(ios::fixed) << setprecision(10);

	cin >> qq;
	forn(ii, qq)
	{
		long long lastArea = 0;
		int counter = 0;
		printf("Case #%d: ", ii+1);
		cin >> X >> S >> R >> t >> N;
		long long Btemp, Etemp, Wtemp;
		forn(i, N)
		{
			cin >> Btemp >> Etemp >> Wtemp;
			if ( Btemp > lastArea )
			{
				intervals[counter] = Btemp - lastArea;
				W[counter] = 0;
				counter ++;
			}
			intervals[counter] = Etemp-Btemp;
			W[counter] = Wtemp;
			counter ++;
			lastArea = Etemp;			
		}
		if ( lastArea < X )
		{
			intervals[counter] = X-lastArea;
			W[counter] = 0;
			counter ++;
		}
		N = counter;

		double ans = f();

		cout << ans << endl;
	}

	return 0;
}
