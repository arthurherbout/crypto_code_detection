// osmos.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <algorithm>

int motes [100];
int tests;		//number of tests
int armin;		//size of our mote
int num;		//number of other motes
int optimal;	//the optimal solution
int count;		//how far we advance
int t;
int armin_temp;

void solve_osmos ( );
void iterate ( );


int main( )
{
	
	scanf ( "%i", &tests );

	for ( t = 1; t <= tests; ++t )
	{
		solve_osmos ( );
		printf ( "Case #%i: %i\n", t, optimal );
	}


	return 0;
}

void solve_osmos ( )
{

	//input
	scanf ( "%i %i", &armin, &num );

	for ( int *i = motes, *end = motes + num; i != end; ++i )
	{
		scanf ( "%i", i );
	}

	if ( armin == 1 )
	{
		optimal = num;
		return;
	}
	
	std::sort ( motes, motes + num );
	
	optimal = num;

	int steps = 0;		//num of steps done so far

	do
	{
		iterate ( );
		
//		if ( optimal < num - count )
//			return;
		if ( optimal > num - count + steps )	
			optimal = num - count + steps;
		
		if ( num - count == 0 )
			return;
		
		armin += armin_temp - 1;
		++ steps;

	}	while ( true );

}

void iterate ( )
{
	count = 0;
	armin_temp = armin;
	//advance to where possible
	for ( int *i = motes, *end = motes + num; i != end; ++i, ++count )
	{
		if ( armin_temp <= *i )
			break;
		armin_temp += *i;
	}
	

}
