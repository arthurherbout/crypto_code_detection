#include "stdio.h"
#include "gmpxx.h"
#include <iostream>
using namespace std;

mpz_class p ( mpz_class a , mpz_class b )
{
	mpz_class r = 1;
	for( mpz_class i = 0 ; i < b ; i++ )
		r *= a;
	return r;
}

int main()
{
	int T;
	cin >> T;

	for( int test = 1 ; test <= T ; test++ )
	{
		mpz_class L,P;
		mpz_class C;
		cin >> L >> P >> C;
	
		mpz_class k;
		for( k = 0 ; ; k++ )
		{
			if( L * p( C , p( 2 , k ) ) >= P )
				break;
		}
		cout << "Case #"<<test << ": "<< k <<endl;
	
	}



}
