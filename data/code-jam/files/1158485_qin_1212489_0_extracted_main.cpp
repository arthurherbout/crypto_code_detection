#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<math.h>
#include<map>

using namespace std;

__int64 mask;
vector<int> pos;
int n;

bool succ_it( __int64 now )
{
	__int64 s = 1;
	__int64 t = 2000000000;
	
	while( s <= t )
	{
		__int64 mid = (s+t)/2;
		if( mid * mid == now )
			return true;
		if( mid * mid > now )
			t = mid - 1 ;
		else
			s = mid + 1;
	}
	return false;
}

__int64 test_it( int p )
{
	__int64 now = mask;
	for( int i = 0; i < n; ++i )
		if( p & (1<<i ) )
			now |= (((__int64)1)<<pos[i]);
	if( succ_it( now ) )
		return now;
	return (__int64)(-1);
}

void put_it( __int64 s )
{
	if( s == 0 )
		return;
	put_it( s / 2 );
	if( s & 1 )
		putchar( '1' );
	else
		putchar( '0' );
}

void make_it()
{
	mask = 0;
	pos.clear();
	char buf[1024];
	scanf( "%s", &buf );
	int len = (int) strlen( buf );
	for( int i = 0; i < len; ++i )
	{
		char ch = buf[len-1-i];
		if( ch == '1' )
			mask |= ( ((__int64)1) << i );
		else if( ch == '?' )
			pos.push_back( i );
	}

	n = (int) pos.size();
	for( int i = 0; i < (1<<n); ++i )
	{
		__int64 rst = test_it( i );
		if( rst > 0 )
		{
			put_it( rst );
			printf( "\n" );
			return;
		}
	}
}

int main()
{
	freopen( "D-small-attempt0 (1).in", "r", stdin );
	freopen( "a.out", "w", stdout );
	int test_case;
	scanf( "%d", &test_case );

	for( int i = 1; i <= test_case; ++i )
	{
		printf( "Case #%d: ", i );
		make_it();
	}

	return 0;
}