#include "stdafx.h"
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#define DIGIT(c) ((c) >= '0' && (c) <= '9')
#define OFF 0
#define ON 1
char Word[10],line[20];

int ReadNumber(FILE *fp)
{
	int num = 0;
    
	memset(Word,0,sizeof(Word));
	int i = 0;
	char c = '\0';

	c = fgetc(fp);
	
	if(!DIGIT(c))
	{
		do
		{
			c = fgetc(fp);
		}while(!DIGIT(c));
	}
	
	do
	{
		Word[i++] = c;
		c = fgetc(fp);

	}while(DIGIT(c));

	num = atoi(Word);

	return num;
}

void print(FILE *fp,int index,int success)
{
	char *state = "ON";
	memset(line,0,sizeof(line));

	if(!success)
		state = "OFF";

	sprintf(line,"Case #%d: %s",index+1,state);
	fputs(line,fp);
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	int n = 0, tc = 0, s = 0, powern = 0;
	int i = 0;
	FILE *fp_in = NULL, *fp_out = NULL;
	
	fp_in = fopen("a.in","r");
	fp_out = fopen("a.out","w");

	tc = ReadNumber(fp_in);

	for(i=0;i<tc;i++)
	{
		n = ReadNumber(fp_in);
		s = ReadNumber(fp_in);
		

		if(i != 0)
		{
			fputc('\n',fp_out);
		}
		if(s && (s & s-1))
		{
			powern = 1 << n;
			if(s % powern == (powern-1))
			{
				print(fp_out,i,ON);
			}
			else
			{
				print(fp_out,i,OFF);
			}
		}
		else if(n == 1 && s == 1)
		{
			print(fp_out,i,ON);
		}
		else
		{
			print(fp_out,i,OFF);
		}

	}

	fclose(fp_in);
	fclose(fp_out);

	return 0;
}

