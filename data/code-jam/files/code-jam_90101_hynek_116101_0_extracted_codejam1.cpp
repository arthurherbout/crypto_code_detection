//code jam ?.1
#include <stdio.h>
#include <stdlib.h>

#define MAXD 5000
#define MAXL 15
#define MAXN 500

char Dictionary[MAXD][MAXL + 1];
int Satisfies[MAXD];
int Letter[MAXL][26][MAXD];
int nLetter[MAXL][26];

char Word[MAXL][26];
int nWord[MAXL];

int main()
{
	//fclose(stdin);
	//stdin=fopen("A-small-attempt0.in","r");
	
	
	int d, l, n;
	scanf("%d%d%d", &l, &d, &n);
	
	for(int i = 0; i < d; i++)
	{
		scanf("%s", Dictionary[i]);
	}
	
	for(int i = 0; i < d; i++)
	{
		for(int j = 0; j < l; j++)
		{
			Letter[j][Dictionary[i][j] - 'a'][nLetter[j][Dictionary[i][j] - 'a']++] = i;
		}
	}
	
	char c;
	for(int count = 0; count < n; count++)
	{
		for(int i = 0; i < l; i++)
		{
			nWord[i] = 0;
		}
		
		int j = 0;
		
		getchar();
		while(j < l)
		{
			c = getchar();
			
			if(c == '(')
			{
				c = getchar();
				while(c != ')')
				{
					Word[j][nWord[j]++] = c;
					c = getchar();
				}
			}
			else
			{
				Word[j][nWord[j]++] = c;
			}
			
			j++;
		}
		
		for(int i = 0; i < d; i++)
		{
			Satisfies[i] = 0;
		}
		
		for(int i = 0; i < l; i++)
		{
			for(int j = 0; j < nWord[i]; j++)
			{
				for(int k = 0; k < nLetter[i][Word[i][j] - 'a']; k++)
				{
					Satisfies[Letter[i][Word[i][j] - 'a'][k]]++;
				}
			}
		}
		
		int rsl = 0;
		for(int i = 0; i < d; i++)
		{
			if(Satisfies[i] == l)
			{
				rsl++;
			}
		}
		
		printf("Case #%d: %d\n", count + 1, rsl);
	}
	
	return 0;
}
