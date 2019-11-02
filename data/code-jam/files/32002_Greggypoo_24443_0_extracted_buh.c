#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int
read_int(void)
{
	char s[1000];
	if (!fgets(s, 1000, stdin)) {
		perror("fgets");
		exit(-1);
	}
	return atoi(s);
}

char *
read_string(void)
{
	char s[10000];
	if (!fgets(s, 10000, stdin)) {
		perror("fgets");
		exit(-1);
	}
	s[strcspn(s,"\r\n")] = 0;
	return strdup(s);
}
void
read_int2(int *x,int *y)
{
	char s[1000];
	char *p;
	if (!fgets(s, 1000, stdin)) {
		perror("fgets");
		exit(-1);
	}
	p = s;
	*x = strtoul(p, &p, 10);
	p++;
	*y = strtoul(p, &p, 10);
}

void
read_int4(int *x,int *y, int *z, int *a)
{
	char s[1000];
	char *p;
	if (!fgets(s, 1000, stdin)) {
		perror("fgets");
		exit(-1);
	}
	p = s;
	*x = strtoul(p, &p, 10);
	p++;
	*y = strtoul(p, &p, 10);
	p++;
	*z = strtoul(p, &p, 10);
	p++;
	*a = strtoul(p, &p, 10);
}
void
read_double5(double *a,double *b,double*c,double*d,double*e)
{
	char s[1000];
	char *p;
	if (!fgets(s, 1000, stdin)) {
		perror("fgets");
		exit(-1);
	}
	p = s;
	*a = strtod(p, &p);
	p++;
	*b = strtod(p, &p);
	p++;
	*c = strtod(p, &p);
	p++;
	*d = strtod(p, &p);
	p++;
	*e = strtod(p, &p);
}

int
read_intx(char **s)
{
	char *p = *s;
	int ret;
	while (isspace(*p)) {
		p++;
	}
	ret = strtoul(p, &p, 10);
	*s = p;
	return ret;
}
int R,C;
char *grid;
#define GRID(x,y) grid[(x)+(y)*C]
int *cost;
#define COST(x,y) cost[(x)+(y)*C]
int cake_x, cake_y;
#define INF 100000000

int least_cost(int x, int y)
{
	int i,j;
	int northy,eastx,westx,southy;
	int northcost,eastcost,westcost,southcost;
	int min_cost = INF;
	if (x == cake_x && y == cake_y) {
		return 0;
	}
	if ((x < 0) || (y < 0) || (x >= C) || (y >= R)) {
		return INF;
	}
	if (GRID(x,y)) {
		return INF;
	}
	if ((i=COST(x,y)) != -1) {
		return i;
	}
COST(x,y) = INF;
	if ((i = (least_cost(x+1, y)+1)) < min_cost) {
		min_cost = i;
	}
	if ((i = (least_cost(x, y+1)+1)) < min_cost) {
		min_cost = i;
	}
	if ((i = (least_cost(x-1, y)+1)) < min_cost) {
		min_cost = i;
	}
	if ((i = (least_cost(x, y-1)+1)) < min_cost) {
		min_cost = i;
	}
	/* now try portals */
	northy = southy = eastx = westx = -2;
	int done = 0;
	int leastport = INF;
	int leastdir = 0;
	for (i = 1; !done ; i++) {
		done = 1;
		if (northy == -2) {
			if ((i > y) || (GRID(x,y-i))) {
				northy = (y-i)+1;
				northcost = i;
				if (northcost < leastport) {
					leastport = northcost;
					leastdir = 0;
				}
			} else {
				done = 0;
			}
		}
		if (westx == -2) {
			if ((i > x) || (GRID(x-i,y))) {
				westx = (x-i)+1;
				westcost = i;
				if (westcost < leastport) {
					leastport = westcost;
					leastdir = 3;
				}
			} else {
				done = 0;
			}
		}
		if (eastx == -2) {
			if ((i+x >= C) || (GRID(x+i,y))) {
				eastx = (x+i)-1;
				eastcost = i;
				if (eastcost < leastport) {
					leastport = eastcost;
					leastdir = 1;
				}
			} else {
				done = 0;
			}
		}
		if (southy == -2) {
			if ((i+y >= R) || (GRID(x,y+i))) {
				southy = (y+i)-1;
				southcost = i;
				if (southcost < leastport) {
					leastport = southcost;
					leastdir = 1;
				}
			} else {
				done = 0;
			}
		}
	}
	if ((leastdir != 0) &&
	((i = (least_cost(x, northy)+leastport)) < min_cost) ){
		min_cost = i;
	}
	if ((leastdir != 1) &&
	((i = (least_cost(eastx, y)+leastport)) < min_cost)) {
		min_cost = i;
	}
	if ((leastdir != 2) &&
	((i = (least_cost(x, southy)+leastport)) < min_cost)) {
		min_cost = i;
	}
	if ((leastdir != 3) &&
	((i = (least_cost(westx, y)+leastport)) < min_cost)) {
		min_cost = i;
	}
	COST(x,y) = min_cost;
	return min_cost;

}

int main(void)
{
	int N,i,j,k;
	int you_x, you_y;
	N = read_int();
	for (i = 0; i < N; i++) {
		read_int2(&R,&C);
		grid = malloc(sizeof *grid*R*C);
		cost = malloc(sizeof *cost*R*C);
		memset(grid,0, sizeof *grid*R*C);
		memset(cost,0xFF, sizeof *cost*R*C);
		for (j = 0; j < R; j++) {
			char *l = read_string();
			for (k = 0; k < C; k++) {
				if (l[k] == 'X') {
					cake_x = k;
					cake_y = j;
				} else if (l[k] == 'O') {
					you_x = k;
					you_y = j;
				}
				GRID(k,j) = (l[k] == '#');
			}
		}
		j = least_cost(you_x, you_y);
		if (j == INF) {
		printf("Case #%d: THE CAKE IS A LIE\n", i+1);
		} else {
		printf("Case #%d: %d\n", i+1, j);
		}
	}

	return 0;
}
