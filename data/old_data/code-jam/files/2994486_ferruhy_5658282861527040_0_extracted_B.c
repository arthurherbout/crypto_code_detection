/*------------------------------------------------------------------------------
* Filename    :  B.c
* Author      :  
* Description : 
*
* Date        :  May  3 2014 18:59:57
*
*-----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "B.h"
#include <stdio.h>
#include <string.h>

/* Private Types -------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* Function Declerations -----------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* Function    :  
* Description : 
*
* Params      : 
* Returns     : 
*-----------------------------------------------------------------------------*/
static int calculate_small_ones(int a, int b, int k)
{
	int val;
	int mina;
	int minb;

	mina = k;
	if (a < mina) mina = a;
	val = mina * a;

	minb = k;
	if (b < minb) minb = b;
	val += minb * b;

	val -= mina * minb;
	return val;
}

static int calculate(int a, int b, int k)
{
	int i, j;
	int val;

	/*val = calculate_small_ones(a, b, k);*/

	for (i = 0; i < a; i++) {
		for (j = 0; j < b; j++) {
			if ((i & j) < k)
				val++;
		}
	}
	return val;
}

/*------------------------------------------------------------------------------
* Function    :  main 
* Description :  main function 
*
* Params      :  int argc, char *argv[] 
* Returns     :  int 
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	int T, A, B, K;
	int i, j;
	int rc;

	scanf("%d\n", &T);

	for (i = 0; i < T; i++) {
		printf("Case #%d: ", i+1);
		scanf("%d %d %d\n", &A, &B, &K);
		rc = calculate(A, B, K);
		printf("%d\n", rc);
	}

	return 0;
}


/* end of B.c */
