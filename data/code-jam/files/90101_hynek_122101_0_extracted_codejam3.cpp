//code jam ?.3
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATTERN "welcome to code jam"
#define PATTERNL 20
#define MAXL 500
#define MOD 10000

char Pattern[PATTERNL] = PATTERN;
int Coef[PATTERNL];
char String[MAXL + 1];

int main()
{
	int n;
	scanf("%d", &n);
	getchar();
	
	for(int count = 0; count < n; count++)
	{
		Coef[0] = 1;
		for(int i = 1; i < PATTERNL; i++)
		{
			Coef[i] = 0;
		}
		
		gets(String);
		int l = strlen(String);
		
		for(int i = 0; i < l; i++)
		{
			for(int j = 0; j < PATTERNL; j++)
			{
				if(String[i] == Pattern[j])
				{
					Coef[j + 1] = (Coef[j + 1] + Coef[j]) % MOD;
				}
			}
		}
		
		printf("Case #%d: %04d\n", count + 1, Coef[PATTERNL - 1]);
	}

	return 0;
}
