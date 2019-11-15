// fair.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
//#include <iostream>
//#include <sstream>
#include <cstdio>
//#include <cstlib>
//#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	//cin >> a >> b;

	unsigned long long a, b;
	unsigned tests;
	//ss << a;
	//string str = ss.str();
	scanf ( "%u", &tests );
	for ( unsigned t = 1; t <= tests; ++t )
	{

		scanf ( "%llu%llu", & a, & b );
	//	cin >> a >> b;
		
		unsigned counter = 0;
		
		for ( unsigned long long i = 1, j = 1; i <= b && j <= b; ++i, j = i*i )
		{
			if ( j < a )
				continue;
			char * temp = new char [101];
			sprintf ( temp, "%llu", i );
			string str = string ( temp );
			//stringstream ss;
			//ss << i;
			//string str = ss.str();
			//ss.clear();
			if ( str == string ( str.rbegin(), str.rend() ) )
			{
				char *temp2 = new char [101];
				sprintf ( temp2, "%llu", j );
				string str2 = string ( temp2 );
				//stringstream ss1;
				//ss1 << j;
				//string str2 = ss1.str();
				//ss1.clear();
				if ( str2 == string ( str2.rbegin(), str2.rend() ) )
				{
					++counter;
					//printf ( "%llu\n", j );
					//cout << j << endl;
				}
			}
		}

		printf ( "Case #%u: %u\n", t, counter );
		//cout << counter << endl;
	}
	return 0;
}

