// lawnmower.cpp : Defines the entry point for the console application.
// Compiled using VisualC++ 2008 Express compiler

#include "stdafx.h"
#include <cstdio>
#include <cstdlib>

int tests, m, n;

int field [100] [100];
int max_row [100];
int max_column [100];

bool solve ( const int & t );
//void calc ( const int & x, const int & y );

int main( )
{
	
	scanf ( "%d", &tests );

	for ( int t = 1; t <= tests; ++t )
	{
		if ( solve ( t ) )
			printf ( "Case #%d: YES\n", t );
		else
			printf ( "Case #%d: NO\n", t );
	}

	//system ( "pause" );

	return 0;
}

bool solve ( const int & t )
{

	scanf ( "%d%d", &n, &m );

	for ( int j = 0; j < n; ++j )
		for ( int i = 0; i < m; ++i )
			scanf ( "%d", & field [i] [j] );
	
	for ( int j = 0; j < n; ++j )
		max_row [j] = field [0] [j];
	for ( int i = 0; i < m; ++i )
		max_column [i] = field [i] [0];

	//find maximums
	for ( int j = 0; j < n; ++j )
	{
		for ( int i = 0; i < m; ++i )
		{
			if ( max_row [ j ] < field [ i ] [ j ] )
				max_row [ j ] = field [ i ] [ j ];
			if ( max_column [ i ] < field [ i ] [ j ] )
				max_column [ i ] = field [ i ] [ j ];
		}
	}
	
	for ( int j = 0; j < n; ++j )
		for ( int i = 0; i < m; ++i )
			if ( field [ i ] [ j ] < max_row [ j ] && field [ i ] [ j ] < max_column [ i ] )
				return false;

	return true;

}

/*
bool calc ( const int & x, const int & y )
{
	if ( x == -1 || x == m || y == -1 || y == n )
		return true;
	if ( board [ i max_row ( x
}
*/
