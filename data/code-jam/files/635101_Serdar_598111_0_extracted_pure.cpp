#include "stdio.h"
#include "gmpxx.h"

long long int list[501][501];
int combs[501][501];

int comb( int a , int b )
{
	if( a == 0 )
		return 1;

	if( combs[a][b] )
		return combs[a][b];
	
	mpz_class t = 1;
	mpz_class mod = 100003;
	
	for( int i = 1 ; i <= b ; i++ )
	{
		t *= (a-i+1);
		t /= b;
	}
	
	t = t%100003;
	combs[a][b] = t.get_si();
	return combs[a][b];
}

int arr[505];

int main()
{

/*	arr[0] = 0;
	arr[1] = 1;
	for( int i = 2 ; i <= 100 ; i++ )
	{
		arr[i] = arr[i-1] + arr[i-2];
		arr[i] = arr[i] % 100003;
		printf("%d %d\n",i,arr[i]);
	}
	
	int T,n;
	
	scanf(" %d", &T);
	
	for( int i = 1 ; i <= T ; i++ )
	{
		scanf(" %d", &n);
		printf("Case #%d: %d\n",i,arr[n]);
	}*/

	for( int i = 2 ; i <= 25 ; i++ )
	{
		list[i][1] = 1;
	
		for( int j = 2 ; j < i ; j++ )
		{
			for( int fark = 0 ; fark < i-j  ; fark++ )
			{
				//printf("[%d][%d] * C(%d,%d)\n",j,j-1-fark, i-j-1 , fark);
				list[i][j] += list[j][ j - 1 - fark] * comb( i-j-1 , fark );
			}
			//printf("%d %d = %lld\n",i,j,list[i][j]);
			list[i][j] %= 100003;
		}
	
		long long int tot=0;
		for( int j = 1 ; j <i ; j++ )
			tot+= list[i][j];
		//printf("%d %lld\n",i , tot % 100003);
		arr[i] = tot % 100003;
	}

	int T,n;
	
	scanf(" %d", &T);
	
	for( int i = 1 ; i <= T ; i++ )
	{
		scanf(" %d", &n);
		printf("Case #%d: %d\n",i,arr[n]);
	}

	return 0;
}
