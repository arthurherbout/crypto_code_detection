// consonants.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>

int solve ( );

int main ( )
{
	
	int tests;

	scanf ( "%i", &tests );

	for ( int t = 1; t <= tests; ++t )
	{
		//solve ( );
		printf ( "Case #%i: %i\n", t, solve ( ) );
	}


	return 0;
}

int solve ( )
{
	std::string name;
	char namea [110];
	int length, num;
	int smallcounter = 0, bigcounter = 0, smallmax = 0;
	int i, j, k;

	//input
	scanf ( "%s %i", namea, &num );
	//name = std::string (namea);
	length = strlen ( namea );

	//for ( std::string::iterator i = name.begin(); i != 
	for ( i = 0; i < length; ++i )
	{
		for ( k = i + 1; k <= length; ++k )
		{
			smallcounter = 0;
			smallmax = 0;
			for ( j = i; j < k; ++j )
			{
				if ( namea [ j ] != 'a' && namea [ j ] != 'e' && namea [ j ] !=  'i'
					&& namea [ j ] !=  'o' && namea [ j ] != 'u' )
					++smallcounter;
				else
					smallcounter = 0;
				if ( smallcounter > smallmax )
					smallmax = smallcounter;
			}
			if ( smallmax >= num )
				++bigcounter;
		}
	}

	return bigcounter;

}

