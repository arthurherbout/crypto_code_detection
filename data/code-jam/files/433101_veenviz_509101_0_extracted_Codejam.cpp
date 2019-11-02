#include "stdafx.h"
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#define DIGIT(c) ((c) >= '0' && (c) <= '9')
#define SIZE 1000
#define OFF 0
#define ON 1
char Word[15],line[20];
int score[SIZE],index[SIZE],group[SIZE];
FILE *fp_in = NULL, *fp_out = NULL;
int n = 0, tc = 0, repeat = 0, limit = 0, money = 0;

int ReadNumber()
{
	int num = 0;
    
	memset(Word,0,sizeof(Word));
	int i = 0;
	char c = '\0';

	c = fgetc(fp_in);
	
	if(!DIGIT(c))
	{
		do
		{
			c = fgetc(fp_in);
		}while(!DIGIT(c));
	}
	
	do
	{
		Word[i++] = c;
		c = fgetc(fp_in);

	}while(DIGIT(c));

	num = atoi(Word);

	return num;
}

void print(int Case,int money)
{
	sprintf(line,"Case #%d: %d",Case,money);
	fputs(line,fp_out);
}

void Process(int Case)
{
	int i = 0, j = 0, k = 0, sum = 0,count = 0,next = 0;
	
	money = 0;

	for(i=0; i < repeat; i++)
	{
		k = j;
		sum = score[j];
		count = 0;
		next = 0;

		if(index[j] == 0)
		{
			k = (k+1) % n;
			while(sum <= limit && k != j)
			{
				next = group[k];
				sum = sum + next;
				count++;
				k = (k+1) % n;
			}
			
			//if(k == 0 && sum >= limit)
			//	k = n;

			if(sum > limit)
			{
				sum = sum - next;
			}



			score[j] = sum;
			index[j] = count;
			
		}

		money = money + score[j];
		j = (j + index[j]) % n;
		
	}

	//print the money earned
	if(Case != 1)
		fputc('\n',fp_out);

	sprintf(line,"Case #%d: %d",Case,money);
	fputs(line,fp_out);

}

int _tmain(int argc, _TCHAR* argv[])
{
	

	int i = 0, j = 0, k = 0, sum = 0;

	
	fp_in = fopen("c.in","r");
	fp_out = fopen("c.out","w");

	tc = ReadNumber();

	for(i=0;i<tc;i++)
	{
		repeat = ReadNumber();
		limit  = ReadNumber();
		n	   = ReadNumber();
		
		memset(index,0,SIZE);
		memset(group,0,SIZE);
		memset(score,0,SIZE);

		for(j=0;j<n;j++)
		{
			group[j] = score[j] = ReadNumber();
		}
		
		Process(i+1);

	}

	fclose(fp_in);
	fclose(fp_out);

	return 0;
}

