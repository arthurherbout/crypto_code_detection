#include <iostream>
#include <string.h>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <queue>
#include <map>
#include <vector>
#define phb push_back
#define ppb pop_back
using namespace std ;
inline int in(int d=0,char q=0,int c=1){while(q!='-'&&q!=EOF&&(q<48||q>57))q=getchar();if(q==EOF)return EOF;if(q=='-')c=-1,q=getchar();do d=d*10+(q^48),q=getchar();while(q<58&&q>47);return c*d;}
int n , m ;
struct Dot
{
	string s ;
	int i ;
} ar[ 51 ] ;
bool Map[ 51 ][ 51 ] ;
void dfs( int dot , int &k )
{
	if ( k >= n )
	{
		return ;
	}
	while ( 1 )
	{
		if ( Map[ dot ][ ar[ k + 1 ].i ] == true )
		{
			k ++ ;
			dfs( ar[ k ].i , k ) ;
		}
		else
		{
			return ;
		}
	}
}
bool gt( Dot a , Dot b )
{
	return a.s < b.s ;
}
int main()
{
	int t = in() , Case = 1 ;
	while ( t -- )
	{
		n = in() , m = in() ;
		for ( int i = 1 ; i <= n ; i ++ )
		{
			cin >> ar[ i ].s ;
			ar[ i ].i = i ;
			for ( int j = 1 ; j <= n ; j ++ )
			{
				Map[ i ][ j ] = false ;
			}
		}
		for ( int i = 0 , a , b ; i < m ; i ++ )
		{
			a = in() , b = in() ;
			Map[ a ][ b ] = true ;
			Map[ b ][ a ] = true ;
		}
		sort( ar + 1 , ar + n + 1 , gt ) ;
		int cou ;
		string s = ar[ 1 ].s ;
		if ( 0 && Case == 21 )
		{
			for ( int i = 1 ; i <= n ; i ++ )
			{
				for ( int j = 1 ; j <= n ; j ++ )
				{
					cout << Map[ i ][ j ] << ' ' ;
				}
				cout << endl ;
			}
		}
		do
		{
			if ( Case == 21 )
			{
				for ( int i = 1 ; i <= n ; i ++ )
				{
				//	cout << ar[ i ].s << ' ' << ar[ i ].i << ' ' ;
				}
				//cout << endl ;
			}
			cou = 1 ;
			dfs( ar[ 1 ].i , cou ) ;
			if ( Case == 21 )
			{
				//cout << "cou " << cou << endl ;
			}
			if ( cou >= n )
			{
				if ( s != ar[ 1 ].s )
				{
					//cout << "**********************************\n" ;
				}
				break ;
			}
		}
		while ( next_permutation( ar + 1 , ar + n + 1 , gt ) ) ;
		printf( "Case #%d: " , Case ++ ) ;
		for ( int i = 1 ; i <= n ; i ++ )
		{
			cout << ar[ i ].s ;
		}
		cout << endl ;
	}
}


