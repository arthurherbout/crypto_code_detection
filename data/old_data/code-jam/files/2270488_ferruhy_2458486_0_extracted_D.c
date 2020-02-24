/*------------------------------------------------------------------------------
* Filename    :  D.c
* Author      :  
* Description : 
*
* Date        :  Apr 13 2013 14:19:08
*
*-----------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "D.h"
#include <stdio.h>
#include <string.h>

/* Private Types -------------------------------------------------------------*/

/* Defines -------------------------------------------------------------------*/
#define MAX_K 50
#define MAX_N 20

//#define DEBUG

/* Function Declerations -----------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/
int keys[MAX_K];
int keys_map[MAX_K];
int chest[MAX_N];
int key_list[MAX_K][MAX_N];
int order[MAX_N];
int order_index;
int complete;

/* Functions -----------------------------------------------------------------*/
void print_chest(int N)
{
	int n;

	printf("CHEST\n");
	for (n = 0; n < N; n++) {
		printf("%d ", chest[n]);
	}
	printf("\n");
}

static int next_chest(int *keys, int *chest, int key_count, int chest_num)
{
	int local_keys[MAX_K];
	int local_chest[MAX_N];
	int local_key_count = 0;

	int i, j, k, x;
	int ret;
	int opened;
	int all_done = 1;
	int min = 0;
	int min_index = 0;
	int tmp;

#ifdef DEBUG
	printf("next_chest chest_num:%d key_count:%d order_index:%d\n", chest_num, key_count, order_index);
#endif

#ifdef DEBUG
	print_chest(chest_num);
#endif

	for (j = 0; j < chest_num; j++)
		if (chest[j] != 0)
			all_done = 0;
	if (all_done) {
		complete = 1;
		return 0;
	}

	if (key_count == 0)
		return 1;


	for (i = 0; i < key_count; i++) {
		keys_map[i] = 0;
		for (j = 0; j < chest_num; j++) {
			if (keys[i] == chest[j]) {
				keys_map[i] = j;
				break;
			}
		}
	}
	for (i = 0; i < key_count; i++) {
		min = keys_map[i];
		min_index = i;
		for (j = i; j < key_count; j++) {
			if (keys_map[j] < min) {
				min = keys_map[j];
				min_index = j;
			}
		}
		tmp = keys_map[i];
		keys_map[i] = keys_map[min_index];
		keys_map[min_index] = tmp;
		tmp = keys[i];
		keys[i] = keys[min_index];
		keys[min_index] = tmp;
	}


	for (i = 0; i < key_count; i++) {
		for (j = 0; j < chest_num; j++) {
			if (chest[j] == 0) continue;
			/* Save world */
			local_key_count = key_count;
			for (k = 0; k < MAX_K; k++) local_keys[k] = keys[k];
			for (k = 0; k < MAX_N; k++) local_chest[k] = chest[k];
#ifdef DEBUG
			printf("j:%d key count:%d\n", j, key_count);
#endif

			/* open chest */
			if (keys[i] == chest[j]) {
				opened = 1;
				chest[j] = 0;
				keys[i] = 0;
				k = i;
				while (k < key_count) {
					keys[k] = keys[k+1];
					k++;
				}
				key_count--;
				k = 0;
				x = 0;
				while (key_list[k][j]) {
#ifdef DEBUG
					printf("key_list:%d\n", key_list[k][j]);
#endif
					while (keys[x]) {
//						printf("keys[%d]:%d\n", x, keys[x]);
						x++;
					}
#ifdef DEBUG
//					printf("key_count:%d x:%d k:%d\n", key_count, x, k);
#endif
					keys[x] = key_list[k][j];
					key_count++;
#ifdef DEBUG
//					printf("key_count:%d x:%d k:%d\n", key_count, x, k);
#endif
					k++;
				}

			} else {
				opened = 0;
			}
#ifdef DEBUG
			printf("j:%d key count:%d\n", j, key_count);
#endif
			if (!opened) {
				/* Load world back */
				key_count = local_key_count;
				for (k = 0; k < MAX_K; k++) keys[k] = local_keys[k];
				for (k = 0; k < MAX_N; k++) chest[k] = local_chest[k];
			} else {
				order[order_index++] = j;
#ifdef DEBUG
				printf("J:%d\n", j);
#endif
				ret = next_chest(keys, chest, key_count, chest_num);
				if (ret) {
					/* Load world back */
					key_count = local_key_count;
					for (k = 0; k < MAX_K; k++) keys[k] = local_keys[k];
					for (k = 0; k < MAX_N; k++) chest[k] = local_chest[k];
					order_index--;
				}
#ifdef DEBUG
				print_chest(chest_num);
#endif
			}
		}
	}

	return 0;
}

void print_order(int N)
{
	int n;

//	printf("ORDER\n");
	if (complete == 0) {
		printf("IMPOSSIBLE\n");
		return;
	}

	for (n = 0; n < N; n++) {
		printf("%d ", order[n] + 1);
	}
	printf("\n");
}

void parse_chest(int N, int K)
{
	complete = 0;
	order_index = 0;
	next_chest(keys, chest, K, N);
	print_order(N);
}

void print_key_list(int N, int max_ki)
{
	int i, j;

	printf("KEY LIST\n");
	for (i = 0; i < max_ki; i++) {
		for (j = 0; j < N; j++) {
			printf("%d ", key_list[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}

void print_keys(int K)
{
	int i;
	printf("KEYS\n");
	for (i = 0; i < K; i++)
		printf("%d ", keys[i]);
	printf("\n\n");
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
	int K, N, KI;
	int i, k, n, ki;
	char c;
	int max_ki = 0;
	
	scanf("%d\n", &T);

	for (i = 0; i < T; i++) {
		max_ki = 0;
		for (n = 0; n < MAX_N; n++) chest[n] = 0;
		for (k = 0; k < MAX_K; k++) keys[k] = 0;
		for (k = 0; k < MAX_K; k++) {
			for (n = 0; n < MAX_N; n++)
				key_list[k][n] = 0;
		}
		printf("Case #%d: ", i+1);
		scanf("%d %d\n", &K, &N);
		for (k = 0; k < K; k++)
			scanf("%d", &keys[k]);
		scanf("%c", &c);
		for (n = 0; n < N; n++) {
			scanf("%d", &chest[n]);
			scanf("%d", &KI);
			if (KI > max_ki) max_ki = KI;
			for (ki = 0; ki < KI; ki++)
				scanf("%d", &key_list[ki][n]);
			scanf("%c", &c);
		}

#ifdef DEBUG
		print_chest(N);
		print_key_list(N, max_ki);
		print_keys(K);
#endif
		parse_chest(N, K);
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


/* end of D.c */
