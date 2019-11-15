#include <stdlib.h>
#include <stdio.h>

int numCase;
int row, col;
char data[60][60];
int states[100000];
int states2[100000];
int numState, numState2;
int tMax;

int getBit(int state, int n)
{
	if (n < 0 || n > col)
		return 0;
	else return (state & (1 << n));
}

void search(int r, int c)
{
	if (r == row)
		return;
	
	int i;
//	for (i = 0; i < tMax; i++)
//		printf("%d ", states[i]);
//	printf("\n");
	
	for (i = 0; i < tMax; i++)
		states2[i] = -1;
		
	if (data[r][c] != '.')
	{	// update things where this is filled
		for (i = 0; i < tMax; i++)
		{
			if (states[i] == -1)
				continue;
			int newState = i, newVal;
			if (getBit(i, c) == 0)
			{
				newState += (1 << c);
			}
			newVal = states[i] + 4;
			if (getBit(i, c-1) != 0)
				newVal -= 2;
			if (getBit(i, c+1) != 0)
				newVal -= 2;
			if (states2[newState] == -1 || states2[newState] < newVal)
			{
				//printf("%d %d, Put %d into %d\n", r, c, newVal, newState);
				states2[newState] = newVal;
			}
		}
	}
	if (data[r][c] != '#')
	{
		for (i = 0; i < tMax; i++)
		{
			if (states[i] == -1)
				continue;
			int newState = i, newVal = states[i];
			if (getBit(i, c) != 0)
			{
				newState -= (1 << c);
			}
			if (states2[newState] == -1 || states2[newState] < newVal)
			{
				//printf("%d %d, 2Put %d into %d\n", r, c, newVal, newState);
				states2[newState] = newVal;
			}
		}
	}
	if (c == col-1)
	{
		for (i = 0; i < tMax; i++)
			states[i] = -1;
		for (i = 0; i < tMax; i+=2)
		{
			states[i] = states2[i/2];
			if (states[i] == -1 || states[i] < states2[i/2+tMax/2])
				states[i] = states2[i/2+tMax/2];
		}
		search(r+1, 0);
	}
	else
	{
		for (i = 0; i < tMax; i++)
		{
			states[i] = states2[i];
		}
		search(r, c+1);
	}
//	for (i = 0; i < tMax; i++)
//		printf("%d ", states[i]);
//	printf("\n");
}


int main()
{
	scanf("%d", &numCase);
	int i, j;
	for (i = 0; i < numCase; i++)
	{
		scanf("%d %d", &row, &col);
		//printf("%d %d\n", row, col);
		tMax = (1 << (col+1));
		for (j = 0; j < row; j++)
		{
			scanf("%s", data[j]);
		}
		states[0] = 0;
		for (j = 1; j < tMax; j++)
			states[j] = -1;
		search(0, 0);
		int maxAns = 0;
		for (j = 0; j < tMax; j++)
		{
			if (maxAns < states[j])
				maxAns = states[j];
		}
		printf("Case #%d: %d\n", i+1, maxAns);
	}
	return 0;
}
