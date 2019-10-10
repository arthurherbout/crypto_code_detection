#include <vector>
#include <map>
#include <list>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

using namespace std;

#define MAXN 1001

int N;
int d[MAXN];
int n[MAXN];
int w[MAXN];
int e[MAXN];
int s[MAXN];
int dlt_d[MAXN];
int dlt_p[MAXN];
int dlt_s[MAXN];

int h[1000];

void solve_it()
{
	scanf( "%d", &N );
	int minp = 1000000;
	int maxp = -1000000;
	for( int i = 0; i < N; ++i )
	{
		scanf( "%d %d %d %d %d %d %d %d", &d[i], &n[i], &w[i], &e[i], &s[i], &dlt_d[i], &dlt_p[i], &dlt_s[i] );
		if( w[i] < minp )
			minp = w[i];
		if( w[i] + dlt_p[i] * n[i] < minp )
			minp = w[i] + dlt_p[i] * n[i];
		if( e[i] > maxp )
			maxp = e[i];
		if( e[i] + dlt_p[i] * n[i] > maxp )
			maxp = e[i] + dlt_p[i] * n[i];
	}
	maxp -= minp;
	for( int i = 0; i < N; ++i )
	{
		w[i] -= minp;
		e[i] -= minp;
	}

	int num_succ = 0;

	memset( h, 0, sizeof(h) );

	bool succ = true;
	while( succ )
	{
		succ = false;
		int nowd = 1000000000;
		for( int i = 0; i < N; ++i )
			if( n[i] > 0 )
				if( d[i] < nowd )
				{
					nowd = d[i];
					succ = true;
				}
		if( !succ )
			break;
		for( int i = 0; i < N; ++i )
			if( n[i] > 0 && d[i] == nowd )
			{
				bool suc = false;
				for( int j = w[i]; j < e[i]; ++j )
					if( h[j] < s[i] )
					{
						suc = true;
						break;
					}
				if( suc )
					++num_succ;
			}

		for( int i = 0; i < N; ++i )
			if( n[i] > 0 && d[i] == nowd )
			{
				for( int j = w[i]; j < e[i]; ++j )
					if( h[j] < s[i] )
						h[j] = s[i];
				--n[i];
				w[i] += dlt_p[i];
				e[i] += dlt_p[i];
				s[i] += dlt_s[i];
				d[i] += dlt_d[i];
			}
	}

	printf( "%d\n", num_succ );

}

int main()
{
	freopen( "C-small-attempt0.in", "r", stdin );
	freopen( "out.txt", "w", stdout );

	int T;

	scanf( "%d", &T );
	for( int i = 1; i <= T; ++i )
	{
		printf( "Case #%d: ", i );
		solve_it();
	}

	return 0;
}