/* Includes ------------------------------------------------------------------*/
#include "B.h"
#include <stdio.h>
#include <string.h>

/* Private Types -------------------------------------------------------------*/
struct point {
	int x;
	int y;
};

/* Defines -------------------------------------------------------------------*/
#define MAX_S 500

/* Function Declerations -----------------------------------------------------*/

/* Global Variables ----------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/
void jump_east(char *list, int index, int jump, struct point *p)
{
	list[index] = 'E';
	p->x += jump;
}

void jump_west(char *list, int index, int jump, struct point *p)
{
	list[index] = 'W';
	p->x -= jump;
}

void jump_south(char *list, int index, int jump, struct point *p)
{
	list[index] = 'S';
	p->y -= jump;
}

void jump_nort(char *list, int index, int jump, struct point *p)
{
	list[index] = 'N';
	p->y += jump;
}

void solve (struct point *t, struct point *p)
{
	char list[MAX_S] = {0};
	int jump = 1;
	int index = 0;

	while (1) {
		if (t->x != p->x) {
			if (t->x > p->x) {
				if (jump > (t->x - p->x)) {
					jump_west(list, index, jump, p);
				} else {
					jump_east(list, index, jump, p);
				}
			} else {
				if (jump > (p->x - t->x)) {
					jump_east(list, index, jump, p);
				} else {
					jump_west(list, index, jump, p);
				}
			}
			index++;
			jump++;
		}
//		printf("%s jump:%d index=%d P%d:%d T%d:%d\n", list, jump, index, p->x, p->y, t->x, t->y);

		if (t->y != p->y) {
			if (t->y > p->y) {
				if (jump > (t->y - p->y)) {
					jump_south(list, index, jump, p);
				} else {
					jump_nort(list, index, jump, p);
				}
			} else {
				if (jump > (p->y - t->y)) {
					jump_nort(list, index, jump, p);
				} else {
					jump_south(list, index, jump, p);
				}
			}
			index++;
			jump++;
		}
//		printf("%s jump:%d index=%d P%d:%d T%d:%d\n", list, jump, index, p->x, p->y, t->x, t->y);

		if (t->x == p->x && t->y == p->y)
			break;

		if (index >= MAX_S) {
			printf("\n");
			return;
		}

	}

	printf("%s\n", list);
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
	int X, Y;
	int i;
	struct point target;
	struct point pos;

	scanf("%d\n", &T);
	for (i = 0; i < T; i++) {
		printf("Case #%d: ", i+1);
		scanf("%d %d\n", &X, &Y);
		target.x = X;
		target.y = Y;
		pos.x = 0;
		pos.y = 0;

		solve(&target, &pos);
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
