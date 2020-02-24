/*------------------------------------------------------------------------------
* Filename    :  welcome.cpp
* Author      :  
* Description : 
*
* Date        :  Sep  2 2009 20:49:40
*
*-----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "welcome.h"
#include "stdio.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/* Private Types -------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/

/* Function Declerations -----------------------------------------------------*/
int find(char what, int where);

/* Global Variables ----------------------------------------------------------*/
int n;
char msg[550];
char w[30] = "welcome to code jam";
int lw= 19;
int h[19] = {0};

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
	int i;

	scanf("%d", &n);
		gets(msg);
	
	for(i=0; i<n; i++) {
		
		//scanf("%s", msg);
		memset(msg, 0, 550);
		memset(h, 0, 19);
		gets(msg);

		int j = 0, ret =0;
		int count = 0;
		int exit = 0;

		//cout << "msg:" << msg <<  endl;

		while(!exit) {
			while(j<19) {
				ret = find(w[j], h[j]);
				if(ret >= 0) {
					h[j]=ret;
					j++;
					h[j]=ret;
				}
				else {
					break;
				}
			}
			if(j==19) { count++; count = count%10000; }
			if(j>0)
				h[--j]++;
			else
				exit = 1;
		}

		//cout << count << endl;
		//printf("%s\n", msg);
		printf("Case #%d: %04d\n", i+1, count);

	}

	return 0;
} /* main() */ 

int find(char what, int where) 
{
	char *cp = strchr(msg+where, what);

	if(cp == NULL) return -1;

	return cp-msg;
}

/*------------------------------------------------------------------------------
* Function    :  
* Description : 
*
* Params      : 
* Returns     : 
*-----------------------------------------------------------------------------*/


/* vim: set ts=4 sw=4 sta si ff=unix: */


/* end of welcome.c */
