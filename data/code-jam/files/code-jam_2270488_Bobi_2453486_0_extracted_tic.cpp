// tic.cpp : Defines the entry point for the console application.
// Compiled using VisualC++ 2008 Express compiler

#include "stdafx.h"
#include <cstdio>
//#include <cstdlib>

char board [4][4];
int tests;

void solve ( const int & t );

int main( )
{

	scanf ( "%d", &tests );

	for ( int t = 1; t <= tests; ++t )
		solve ( t );

//	system ( "pause" );
	
	return 0;
}

void solve ( const int & t )
{
	int j = 0;
	for ( int i = 0; i < 4; ++i )
	{
		scanf ( "%s", & board [i] );
	}
	printf ( "Case #%d: ", t );
	
	for ( int i = 0; i < 4; ++i )
	{
		//row
		for ( j = 0; j < 4; ++j )
		{
			if ( board [ i ] [ j ] == 'X' || board [ i ] [ j ] == 'T' )
				continue;
			break;
		}
		if ( j == 4 )
		{
			printf ( "X won\n" );
			return;
		}
		for ( j = 0; j < 4; ++j )
		{
			if ( board [ i ] [ j ] == 'O' || board [ i ] [ j ] == 'T' )
				continue;
			break;
		}
		if ( j == 4 )
		{
			printf ( "O won\n" );
			return;
		}

		//column
		for ( j = 0; j < 4; ++j )
		{
			if ( board [ j ] [ i ] == 'X' || board [ j ] [ i ] == 'T' )
				continue;
			break;
		}
		if ( j == 4 )
		{
			printf ( "X won\n" );
			return;
		}
		for ( j = 0; j < 4; ++j )
		{
			if ( board [ j ] [ i ] == 'O' || board [ j ] [ i ] == 'T' )
				continue;
			break;
		}
		if ( j == 4 )
		{
			printf ( "O won\n" );
			return;
		}
	}
	
	//diagonal
	for ( j = 0; j < 4; ++j )
	{
		if ( board [ j ] [ j ] == 'X' || board [ j ] [ j ] == 'T' )
			continue;
		break;
	}
	if ( j == 4 )
	{
		printf ( "X won\n" );
		return;
	}
	for ( j = 0; j < 4; ++j )
	{
		if ( board [ j ] [ j ] == 'O' || board [ j ] [ j ] == 'T' )
			continue;
		break;
	}
	if ( j == 4 )
	{
		printf ( "O won\n" );
		return;
	}

	//reverse diagonal
	for ( j = 0; j < 4; ++j )
	{
		if ( board [ j ] [ 3 - j ] == 'X' || board [ j ] [ 3 - j ] == 'T' )
			continue;
		break;
	}
	if ( j == 4 )
	{
		printf ( "X won\n" );
		return;
	}
	for ( j = 0; j < 4; ++j )
	{
		if ( board [ j ] [ 3 - j ] == 'O' || board [ j ] [ 3 - j ] == 'T' )
			continue;
		break;
	}
	if ( j == 4 )
	{
		printf ( "O won\n" );
		return;
	}
	
	//empty
	for ( int i = 0; i < 4; ++i )
	{
		if ( board [i] [0] == '.' || board [i] [1] == '.' || board [i] [2] == '.' || board [i] [3] == '.' )
		{
			printf ( "Game has not completed\n" );
			return;
		}
		if ( board [0] [i] == '.' || board [1] [i] == '.' || board [2] [i] == '.' || board [3] [i] == '.' )
		{
			printf ( "Game has not completed\n" );
			return;
		}
	}

	//completed and draw
	printf ( "Draw\n" );
}
