#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<iostream>
#include<math.h>

using namespace std;

void main(void)
{
	int n, t = 0;
	long double twon = 0;
	long double k = 0;

	FILE *ifile = fopen("large.in", "r");
	FILE *ofile = fopen("output_large.txt", "w");
	
	if (ifile==NULL)
		printf("Input file not found");
	else if (ofile==NULL)
		printf("Output file not found");
	else
		{
			fscanf(ifile,"%d", &t);
			for (int i = 0; i<t; i++)
			{
				fscanf(ifile, "%d %Lf", &n, &k);
				fprintf(ofile, "Case #%d: ", (i+1));
				
				twon = pow((float)2,n);

				if (fmod((k+1),twon) == 0)
					fprintf(ofile, "ON\n");
				else
					fprintf(ofile, "OFF\n");
			}		
		}
	getche();
}