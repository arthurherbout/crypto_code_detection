#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>

using namespace std;

void reordergroup (int m, int n, long double g[])
{
	long double temp[1000] = {0};
	for (int i=0; i<=m;i++)
		temp[i] = g[i];
	for (int i=m+1; i<n; i++)
		g[i-m-1] = g[i];
	int j = 0;
	for (int i=n-m-1; i<n; i++)
	{
		g[i] = temp[j];
		j++;
	}
}

void main(void)
{
	int t = 0;
	long double r, k, n = 0;
	long double g[1000] = {0};
	int earn = 0;
	long double prsnperride = 0;
	int m = 0;

	FILE *ifile = fopen("small.in", "r");
	FILE *ofile = fopen("output.txt", "w");
	
	if (ifile==NULL)
		printf("Input file not found");
	else if (ofile==NULL)
		printf("Output file not found");
	else
		{
			fscanf(ifile, "%d", &t);
			for (int i=0; i<t;i++)
			{
				fscanf(ifile, "%Lf %Lf %Lf\n", &r, &k, &n);
				//printf("R=%Lf K=%Lf N=%Lf\n", r, k, n);
				for (int j=0; j<n; j++)
				{
					fscanf(ifile, "%Lf ", &g[j]);
					//printf("g[%d] = %Lf\n", j, g[j]);
				}
				//Calculate Euro earning for this test case
				earn = 0; 
				for (int ride =0; ride<r; ride++) // for each ride
				{
					prsnperride = 0; // earning or persons per ride
					m = 0;				// index to help reorder the group queue
					for(int j=0; j<n; j++) // allow possible groups in the queue
					{
						if ((g[j]+prsnperride)<=k)
						{
							prsnperride = prsnperride + g[j];
							m = j;
						}
						else
							break;
					}
					earn = earn + prsnperride; // add earning for this ride
					//printf("earn = %d\n", earn);
					reordergroup(m, n, g);//reorder group queue as per m
				}
				fprintf(ofile, "Case #%d: ", (i+1));
				fprintf(ofile, "%d\n", earn);
			}
		}
	getche();
}