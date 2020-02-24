#include "gmp.h"
#include "stdio.h"

int main()
{

	int C,N;
	mpz_t num , num2  ,gcd , diff , mod ;
	mpz_init(num);
	mpz_init(num2);
	mpz_init(gcd);
	mpz_init(mod);
	mpz_init( diff );

	scanf( " %d" , &C );

	for( int i = 1 ; i <= C ; i++ )
	{

		scanf(" %d" , &N);

		gmp_scanf(" %Zd", num );

		for ( int j = 2 ; j <= N ; j++ )
		{
			gmp_scanf(" %Zd" , num2 );
			mpz_sub( diff , num2 , num );

			if( j == 2 )
				mpz_set( gcd , diff );
			else
				mpz_gcd( gcd , gcd , diff );

			mpz_set( num , num2 );
		}

		mpz_abs( gcd , gcd );
	//	gmp_printf("--- %Zd\n",gcd);
		
		//last num will be enough
		mpz_fdiv_r( mod , num , gcd );//gmp_printf("--- %Zd\n",mod);
		if( mpz_cmp_si( mod , 0 ) == 0 )
		{
			gmp_printf("Case #%d: 0\n",i);
		}
		else
		{
			mpz_sub( diff , gcd , mod ); 
			gmp_printf("Case #%d: %Zd\n",i,diff);
		}
	}


	return 0;
}
