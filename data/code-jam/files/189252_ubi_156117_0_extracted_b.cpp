#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define DEBUG

struct node
{
	double x, y, z, vx, vy, vz;
};

void getAvr(struct node *pNode, struct node* avr, int N)
{
	int j;
	for (j = 0; j < N; ++j)
	{
		(*avr).x += pNode[j].x;
		(*avr).y += pNode[j].y;
		(*avr).z += pNode[j].z;
		(*avr).vx += pNode[j].vx;
		(*avr).vy += pNode[j].vy;
		(*avr).vz += pNode[j].vz;
	}
	(*avr).x /= (double)N;
	(*avr).y /= (double)N;
	(*avr).z /= (double)N;
	(*avr).vx /= (double)N;
	(*avr).vy /= (double)N;
	(*avr).vz /= (double)N;
}

int main()
{
	int T, N, i, j;
	struct node * pNode;
	struct node avr;
	freopen("B-small-attempt1.in","r",stdin);
	freopen("B-small-attempt1.out.txt","w",stdout);

	scanf("%d", &T);
	for (i = 0; i < T; ++i)
	{
		// 1.input
		scanf("%d", &N);
		pNode = (struct node *)calloc(1, sizeof(struct node) * N);
		memset(&avr, 0, sizeof(struct node));
		for (j = 0; j < N; ++j)
			scanf("%lf%lf%lf%lf%lf%lf", &pNode[j].x, &pNode[j].y, &pNode[j].z, &pNode[j].vx, &pNode[j].vy, &pNode[j].vz);
		// 2.get center & avr speed
		getAvr(pNode, &avr, N);
		// 3.get len
		double speed2 = avr.vx * avr.vx + avr.vy * avr.vy + avr.vz * avr.vz;
		double srd2 = avr.x * avr.x + avr.y * avr.y + avr.z * avr.z;
		if (speed2 == 0.0)
		{
			printf("Case #%d: %.8lf %.8lf\n", i + 1, sqrt(srd2), 0.0);
			continue;
		}
		double sd2 = avr.x * avr.vx + avr.y * avr.vy + avr.z * avr.vz;
		double len2 = sd2 * sd2 / speed2;
		// 4.get dmin * dmin
		double dmin2 = srd2 - len2;
		// len / avrSpeed = tmin
		double tmin = sqrt(len2) / sqrt(speed2);
#ifdef DEBUG
		printf("avr: %lf %lf %lf %lf %lf %lf\n", avr.x, avr.y, avr.z, avr.vx, avr.vy, avr.vz);
		printf("speed2: %lf sd2: %lf\n", speed2, sd2);
		printf("tmin: %lf, dmin2: %lf\n", tmin, dmin2);
#endif
		if (dmin2 > 0.0)
			printf("Case #%d: %.8lf %.8lf\n", i + 1, sqrt(dmin2), tmin);
		else
			printf("Case #%d: %.8lf %.8lf\n", i + 1, 0.0, tmin);
		free(pNode);
	}
	return 0;
}