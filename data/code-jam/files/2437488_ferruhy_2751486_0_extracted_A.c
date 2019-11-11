/*------------------------------------------------------------------------------
* Filename    :  A.c
* Author      :  
* Description : 
*
* Date        :  Apr 13 2013 14:19:08
*
*-----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "A.h"
#include <stdio.h>
#include <string.h>

/* Private Types -------------------------------------------------------------*/
struct substr {
	int start;
	int end;
};

/* Defines -------------------------------------------------------------------*/
#define MAX_L 1000000

/* Function Declerations -----------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/
char str[MAX_L];
char vowels[] = {'a', 'e', 'i', 'o', 'u'};

/* Functions -----------------------------------------------------------------*/
int is_valid(struct substr *sstr, int substr_length)
{
	int i, j;
	int max = 0;
	int has_vowel;

	for (i = sstr->start; i < sstr->end; i++) {
		has_vowel = 0;
		for (j = 0; j < sizeof(vowels); j++) {
			if (str[i] == vowels[j]) {
				has_vowel = 1;
				break;
			}
		}
		if (has_vowel)
			max = 0;
		else
			max++;
		if (max >= substr_length)
			return 1;
	}
	return 0;

}

int find_substr(int max_length, int substr_length)
{
	int i;
	int len = max_length;
	struct substr sstr;
	int ret = 0;

	while (len >= substr_length) {
		for (i = 0; i + len <= max_length; i++) {
			sstr.start = i;
			sstr.end = i+len;
			ret += is_valid(&sstr, substr_length);
		}
		len--;
	}

	return ret;

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
	int T;
	int N;
	int i;
	char c;
	int length;
	int ret;

	scanf("%d\n", &T);
	for (i = 0; i < T; i++) {
		printf("Case #%d: ", i+1);

		length = 0;
		c = 0;
		while (c != ' ') {
			scanf("%c", &c);
			str[length++] = c;
		}
		length--;
		str[length] = 0;
		scanf("%d\n", &N);
//		printf("%s %d\n", str, N);

		ret = find_substr(length, N);
		printf("%u\n", ret);
	}

	return 0;
} /* main() */ 

/*------------------------------------------------------------------------------
* Function    :  
* Description : 
*
* Params      : 
* Returns     : 
*-----------------------------------------------------------------------------*/


/* end of A.c */
