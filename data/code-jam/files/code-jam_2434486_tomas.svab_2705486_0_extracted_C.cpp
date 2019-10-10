#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <sstream>
#include <utility>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <bitset>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <cmath>
#include <ctime>
#include <assert.h>
#include <string.h>
#include <limits>
#include <fstream>
using namespace std;

#define INF 100000000
#define PI 3.1415926535897932384626433832795
#define PI_HALF 1.570796327
#define EPS 1e-9

typedef unsigned int UI;
typedef long long LL;
typedef unsigned long long ULL;
typedef pair< int, int > PII;
typedef pair< LL, LL > PLL;
typedef vector< int > VI;
typedef vector< LL > VLL;
typedef vector< string > VS;
typedef vector< PII > VPII;
typedef vector< PLL > VPLL;

#define X first
#define Y second
#define PB push_back
#define MP make_pair
#define SZ(c) (int)(c).size()
#define ALL(c) (c).begin(), (c).end()
#define SORT(c) sort( ALL( c ) )
#define SORT_(c,f) sort( ALL( c ), f )
#define FILL(x,v) memset( x, v, sizeof( x ) );

#define REP(i,n) for ( int i = 0, max_ = (n); i < max_; ++i )
#define REPD(i,n) for ( int i = (n) - 1; i >= 0; --i )
#define FOR(i,a,b) for ( int i = (a), max_ = (b); i < max_; ++i )
#define FORD(i,a,b) for ( int i = (a) - 1, min_ = (b); i >= min_; --i )

int ToInt ( string & s, int len ) { int r = 0; REP(i,len) { r *= 10; r += s[i] - '0'; } return r; }
int GCD ( int a, int b ) { return b != 0 ? GCD(b, a % b) : a; }
int LCM ( int a, int b ) { return a*(b / GCD(a,b)); }
LL Pow ( LL n, LL e ) { if ( e == 0 ) return 1; if ( e == 1 ) return n; else if ( e & 1 ) { LL t = Pow(n,e/2); return n*t*t; } else { LL t = Pow(n,e/2); return t*t; } }

int currID = 0;

struct Node
{
	Node ()
	{
		REP(i,26) n[ i ] = 0;
		end = false;
		id = currID++;
	};
	Node * n[ 26 ];
	bool end;
	int id;
};

Node * root;

void Add ( Node * n, int id, string & s )
{
	if ( id == SZ(s) )
	{
		n->end = true;
		return;
	}

	int nid = s[ id ] - 'a';
	if ( n->n[ nid ] == 0 )
		n->n[ nid ] = new Node();
	Add( n->n[ nid ], id + 1, s );
}

int dp[ 1150000 ][ 51 ][ 8 ];

int BC ( Node * n, int id, string & s, int prev )
{
	if ( id == SZ(s) )
	{
		if ( n->end )
			return 0;
		else
			return INF;
	}
	if ( dp[ n->id ][ id ][ prev ] != -1 ) return dp[ n->id ][ id ][ prev ];

	int best = INF, next = s[ id ] - 'a';
	if ( n->n[ next ] != 0 )
	{
		best = min( best, BC( n->n[ next ], id + 1, s, min( 7, prev + 1 ) ) );
	}
	if ( n->end )
	{
		best = min( best, BC( root, id, s, prev ) );
	}
	if ( prev >= 4 )
	{
		REP(i,26)
			if ( i != next )
			{
				if ( n->n[ i ] != 0 )
				{
					best = min( best, 1 + BC( n->n[ i ], id + 1, s, 0 ) );
				}
			}
	}

	return dp[ n->id ][ id ][ prev ] = best;
}

int main ()
{
	std::ifstream ifs( "garbled_email_dictionary.txt", std::ifstream::in );

	root = new Node();
	string s;
	while ( getline( ifs, s ) )
	{
		Add( root, 0, s );
	}

	freopen( "C-small-attempt1.in", "r", stdin );
//	freopen( "f.in", "r", stdin );
	freopen( "f.out", "w", stdout );

	int N;
	scanf( "%d", &N );
	getline( cin, s );
	REP(i,N)
	{
		getline( cin, s );
		FILL(dp,-1);
		int res = BC( root, 0, s, 7 );
		printf( "Case #%d: %d\n", i + 1, res );
	}

	return 0;
}