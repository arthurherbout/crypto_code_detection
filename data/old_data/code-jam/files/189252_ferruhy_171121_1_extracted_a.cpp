/*------------------------------------------------------------------------------
* Filename    :  a.cpp
* Author      :  ferruhy
* Description :
*
* Date        :  Sep 13 2009 11:49:14
*
*-----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "a.h"
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

/* Private Types -------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
#define SIZE 62

/* Function Declarations -----------------------------------------------------*/
long long solve(void);

/* Global Variables ----------------------------------------------------------*/
FILE *fin;
int T;
char s[SIZE];
int len;

/* Functions -----------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* Function    :  main
* Description :  main function
*
* Params      :  int argc, char *argv[]
* Returns     :  int
*-----------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
	if(argc != 2) return 9;
	if((fin = fopen(argv[1], "r")) == NULL) return 99;
	char dummy[3];
	int i, j;
	long long ret = 0;

	fscanf(fin, "%d", &T);
	fgets(dummy, 3, fin);

	for(i=0; i<T; i++) {
		j=0;
		char c[1];
		memset(s, 0, sizeof(char)*SIZE);
		while(1) {
			fread(c, sizeof(char), 1, fin);
			if(*c == 0xD) continue;
			if(*c == 0xA) break;
			*(s+j) = *c;
			j++;
		}
		len = j;

		ret = solve();
		cout << "Case #" << i+1 << ": " << ret << endl;
	}

	fclose(fin);
	return 0;
} /* main() */

/*------------------------------------------------------------------------------
* Function    :
* Description :
*
* Params      :
* Returns     :
*-----------------------------------------------------------------------------*/
long long solve(void)
{
	vector<char> used;
	vector<char>::iterator i_used;
	int iv = -1;
	char cv = 0;
	int r[SIZE]={0};
	int i = 0;
	int base = 0;
	long long sum = 0;

	for(i=0; i<len; i++) {
		cv = s[i];
		i_used = find(used.begin(), used.end(), cv);
		if(i_used == used.end()) {
			used.push_back(cv);
			if(iv == -1) iv = 1;
			else if(iv == 1) iv = 0;
			else if(iv == 0) iv = 2;
			else iv++;
			int j;
			for(j=i; j<len; j++) {
				if(s[j] == cv) {
					r[j] = iv;
				}
			}
			base++;
		}
	}
	if(base == 1) base ++;
	/*
	cout << "Base:" << base << " Num:";
	for(i=0; i<len; i++) cout << r[i] << " ";
	cout << endl;
	*/

	for(i=0; i<len; i++) {
		int j = len - 1 -i;
		long long y = 1;
		for(; j>0; j--) y = y* base;
		sum += r[i]*y;
	}

	return sum;
}

/* vim: set ts=4 sw=4 sta si ff=unix: */


/* end of a.cpp */
