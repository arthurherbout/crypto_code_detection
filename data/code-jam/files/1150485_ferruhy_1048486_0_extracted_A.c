/*------------------------------------------------------------------------------
 * Filename    :  A.c
 * Author      :  fyigit
 * Description : 
 *
 * Date        :  May 21 2011 08:24:07
 *
 *----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "A.h"

/* Private Types -------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
#define MAX 100

/* Function Declarations -----------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/
int score[MAX][MAX];
double WP[MAX];
double OWP[MAX];
double OOWP[MAX];


/* Functions -----------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * Function    :  main 
 * Description :  main function 
 *
 * Params      :  int argc, char *argv[] 
 * Returns     :  int 
 *----------------------------------------------------------------------------*/
int main(int argc, char *argv[]) {
	int T, N;
	int i, j, k, l;
	char s[MAX + 1];
	int v, n;
	double v2, n2;
	double result;

	scanf("%d", &T);
	//printf("T:%d\n", T);
	for (i = 0; i < T; i++) {
		memset(score, 0, MAX*MAX);
		memset(WP, 0, MAX);
		memset(OWP, 0, MAX);
		memset(OOWP, 0, MAX);
		scanf("%d", &N);
		//printf("N:%d\n", N);
		for (j = 0; j < N; j++) {
			scanf("%s", s);
			//printf("s:%s\n", s);
			for (k = 0; k < N; k++) {
				//printf("c:%c\n", s[k]);
				switch (s[k]) {
				case '0': score[j][k] = 0;
					break;
				case '1': score[j][k] = 1;
					break;
				case '.': score[j][k] = -1;
					break;
				default:
					exit(-1);
				}
			}
		}
		/*
		printf("score\n");
		for (j = 0; j < N; j++) {
			for (k = 0; k < N; k++) {
				printf("%d ", score[j][k]);
			}
			printf("\n");
		}
		printf("\n");
		*/

		/* calculate WP */
		for (j = 0; j < N; j++) {
			v = n = 0;
			for (k = 0; k < N; k++) {
				if (score[j][k] >= 0) {
					v += score[j][k];
					n++;
				}
			}
			WP[j] = (float)v / (float)n;
		}
		/*
		printf("WP\n");
		for (j = 0; j < N; j++) {
			printf("%f ", WP[j]);
		}
		printf("\n");
		*/

		/* calculate OWP */
		for (j = 0; j < N; j++) {
			v2 = n2 = 0;
			for (k = 0; k < N; k++) {
				if (score[j][k] >= 0) {
					v = n = 0;
					for (l = 0; l < N; l++) {
						if (l == j) continue;
						if (score[l][k] >= 0) {
							v += score[l][k];
							n++;
						}
					}
					v2 += (float)v / (float)n;
					n2++;
				}
			}
			OWP[j] = v2 / n2;
			OWP[j] = 1 - OWP[j];
		}
		/*
		printf("OWP\n");
		for (j = 0; j < N; j++) {
			printf("%f ", OWP[j]);
		}
		printf("\n");
		*/

		/* calculate OOWP */
		for (j = 0; j < N; j++) {
			v2 = n2 = 0;
			for (k = 0; k < N; k++) {
				if (score[j][k] >= 0) {
					v2 += OWP[k];
					n2++;
				}
			}
			OOWP[j] = v2 / n2;
		}
		/*
		printf("OOWP\n");
		for (j = 0; j < N; j++) {
			printf("%f ", OOWP[j]);
		}
		printf("\n");
		*/

		printf("Case #%d:\n", i+1);
		for (j = 0; j < N; j++) {
			result = (0.25 * WP[j]) + (0.5 * OWP[j]) + (0.25 * OOWP[j]);
			printf("%.12f\n", result);
		}
	}

	return 0;
}

/* end of A.c */
