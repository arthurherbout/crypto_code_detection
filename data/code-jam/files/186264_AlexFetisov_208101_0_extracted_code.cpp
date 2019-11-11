// AlexFetisov
// Accepted
// I'm Feeling Lucky!

#define _CRT_SECURE_NO_DEPRECATE
#pragma comment (linker, "/STACK:128000000")

#include <iostream>

void initf()
{
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
}

#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <queue>
//#include <cmath>

using namespace std;

#define fr(i,a,b) for (int i =  ( a ); i <= ( b ); ++i)
#define fi( a ) for (int i = 0; i < ( a ); ++i)
#define fj( a ) for (int j = 0; j < ( a ); ++j)
#define fk( a ) for ( int k = 0; k < ( a ); ++k )
#define CLR( a, b ) memset( ( a ), ( b ), sizeof( ( a ) ) )
#define clr( a ) CLR( ( a ), 0 )
#define mp make_pair
#define pb push_back
#define all( v ) ( v ).begin(), ( v ).end()

typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef vector < int > vi;
typedef pair < int, int > pii;

const int inf = 1 << 30;

int dp[1505][23][23];
string ds[1505][23][23];
int W, Q;

bool valid( int x, int y )
{
	return ( x >= 0 && y >= 0 && x < W && y < W );
}

int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};
char a[55][55];

void rel( string & a, const string & b)
{
	if ( b.find( 'z' ) < b.length() ) return;
	if ( a[0] == 'z')
		a = b;
	else
	{
		if (a.size() == 0 || (a.size() > b.size() || a.size() == b.size() && a > b))
			a = b;
	}
}

string rec( int n, int x, int y )
{
	if ( n == 0 ) return ( "" );
	if ( n > 500 || n < -499 ) return ( "z" );
	int & ret = dp[n+500][x][y];

	if ( ret == -2 ) return ( "z" );
	if ( ret != -1 ) return ( ds[n+500][x][y] );
	ret = -2;
	string res = "z";
	vector < pair < pair < char, char >, pair < int, int > > > v;
	
	fi( 4 )
	{
		int nx = x + dx[i], ny = y + dy[i];
		if ( !valid( nx, ny ) ) continue;
		fj( 4 ) 
		{
			int nx1 = nx + dx[j], ny1 = ny + dy[j];
			if (!valid( nx1, ny1 ) ) continue;
			v.pb( mp( mp( a[nx][ny], a[nx1][ny1] ), mp(nx1, ny1 ) ) );	
		}
	}
	sort( all ( v ) );
	fi( v.size() )
	{
		int nn = n;
		if ( v[i].first.first == '+' )
			nn -= v[i].first.second - '0';
		else
			nn += v[i].first.second;
		string t;
		t.pb( v[i].first.first );
		t.pb( v[i].first.second );
		t += rec( nn, v[i].second.first, v[i].second.second );
		
		rel(res, t);
	}
	ret = res.size();
	ds[n+500][x][y] = res;
	return ( res );
}

void solve()
{
	int tst;
	cin >> tst;
	fr(t,1,tst)
	{
		printf("Case #%d:\n", t );
		///////////////////////////////
		cin >> W >> Q;
		fi( W ) fj( W ) cin >> a[i][j];
		CLR( dp, -1 );
		fi( Q )
		{
			int x;
			cin >> x;
			string best = "z";
			fj( W ) fk( W ) if ( a[j][k] != '+' && a[j][k] != '-' )
			{
				string tt;
				tt.pb( a[j][k] );
				tt += rec( x - ( a[j][k] - '0' ), j, k );
				rel(best, tt);
			}
			cout << best << endl;
		}
	}
}

int main()
{
    initf();
    solve();
    return ( 0 );
}
