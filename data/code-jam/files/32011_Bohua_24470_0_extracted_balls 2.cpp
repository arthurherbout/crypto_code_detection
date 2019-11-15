#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

int numCase;
int nx, ny;
int szx, szy;
int lx, ux, ly, uy;
int x1, yy1, x2, y2;
double dmin = 1e-10;
const int MAXSZ = 200;
int fillTable[MAXSZ+1];
int stack[MAXSZ+1];
int sizeStack;


int minShare()
{
	for (int i = 1; i < 100; i++)
	{
		if ((x1 % i == 0) && (yy1 % i == 0) && (x2 == 0 || (x2 % (x1 / i) == 0)) &&
			(y2 == 0 || (y2 % (yy1 / i) == 0)))
			return i;
	}
	printf("Error\n");
	return -1;
}
int min(int a, int b)
{
	if (a < b) return a;
	return b;
}

int max(int a, int b)
{
	if (a < b) return b;
	return a;
}

int main()
{
	scanf("%d", &numCase);
	int i, j;
	for (i = 0; i < numCase; i++)
	{
		scanf("%d %d %d %d %d %d", &szx, &szy, &x1, &yy1, &x2, &y2);
		scanf("%d %d", &nx, &ny);
		lx = -nx;
		ly = -ny;
		ux = szx - nx - 1;
		uy = szy - ny - 1;
		long long count = 0;
		if (x1 * y2 == x2 * yy1)
		{
			int factor = minShare();
			//printf("%d\n", factor);
			int dx = x1 / factor;
			int dy = yy1 / factor;
			int mul1 = factor;
			int mul2 = x2 / (x1 / factor);
			int lower = 0, upper = MAXSZ;
			if (dx > 0)
			{
				lower = (int)ceil(lx * 1.0 / dx - dmin);
				upper = (int)floor(ux * 1.0 / dx + dmin);
			}
			if (dx < 0)
			{
				lower = (int)floor(ux * 1.0 / dx + dmin);
				upper = (int)ceil(lx * 1.0 / dx - dmin);
			}
			if (dy > 0)
			{
				lower = max(lower,(int)ceil(ly * 1.0 / dy - dmin));
				upper = min(upper,(int)floor(uy * 1.0 / dy + dmin));
			}
			if (dy < 0)
			{
				lower = max(lower,(int)floor(uy * 1.0 / dy + dmin));
				upper = min(upper,(int)ceil(ly * 1.0 / dy - dmin));
			}
			//printf("%d %d %d %d\n", dx, dy, lower, upper);
			int offset = -lower;
			lower += offset;
			upper += offset;
			for (j = 0; j <= upper; j++)
			{
				fillTable[j] = 0;
			}
			count = 1;
			fillTable[offset] = 1;
			sizeStack = 1;
			stack[0] = offset;
			while (sizeStack != 0)
			{
				int curPos = stack[sizeStack-1];
				sizeStack--;
				if (curPos + mul1 >= 0 && curPos + mul1 <= upper && fillTable[curPos+mul1] == 0)
				{
					count++;
					fillTable[curPos+mul1] = 1;
					stack[sizeStack++] = curPos+mul1;
				}
				if (curPos + mul2 >= 0 && curPos + mul2 <= upper && fillTable[curPos+mul2] == 0)
				{
					count++;
					fillTable[curPos+mul2] = 1;
					stack[sizeStack++] = curPos+mul2;
				}
			}
		}
		else
		{
			for (j = 0; j <= MAXSZ; j++)
			{
				int lower = 0, upper = MAXSZ;
				if (x2 > 0)
				{
					lower = (int)ceil((lx - j * x1) * 1.0 / x2 - dmin);
					upper = (int)floor((ux - j * x1) * 1.0 / x2 + dmin);
				}
				else if (x2 < 0)
				{
					upper = (int)ceil((lx - j * x1) * 1.0 / x2 - dmin);
					lower = (int)floor((ux - j * x1) * 1.0 / x2 + dmin);
				}
				if (y2 > 0)
				{
					lower = max(lower, (int)ceil((ly - j * yy1) * 1.0 / y2 - dmin));
					upper = min(upper, (int)floor((uy - j * yy1) * 1.0 / y2 + dmin));
				}
				else if (y2 < 0)
				{
					upper = min(upper, (int)ceil((ly - j * yy1) * 1.0 / y2 - dmin));
					lower = max(lower, (int)floor((uy - j * yy1) * 1.0 / y2 + dmin));
				}
				lower = max(lower, 0);
				//printf("%d %d\n", lower, upper);
				if (lower <= upper)
					count += upper - lower + 1;
			}
		}
	
		cout << "Case #" << (i+1) << ": " << count << endl;
	}
	return 0;
}
