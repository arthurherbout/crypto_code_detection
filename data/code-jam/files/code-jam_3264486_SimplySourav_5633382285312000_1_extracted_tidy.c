#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//void resetArray (char *);

int main(void)
{
	int testcase = -1;

	scanf("%d%*c", &testcase);

	//here is the logic for all testcases
	for (int i = 0; i < testcase; i++)
	{
		//some fuckall logic

		char result [64] = {0}; //should be able to hold 10^18

		fgets(result, sizeof(result), stdin);  //read input number
		result[strcspn(result,"\r\n")] = 0;		// I wont tell you what I'm doing here

		int length = strlen(result);

		if (!(length < 2)) //don't process a single digit 
		{
			for (int j = length-1; j-1 >= 0; j--)
			{
				if (result[j] < result[j-1])
				{
					for (int x = j; result[x]; x++) result[x] = '9';
					result[j-1] -= 1;
				//	printf("##########after resetArray %s\n", result);
				}
			}
		}
		//result
		if (result[0] == '0')
			printf ("Case #%d: %s\n", i+1, result+1);
		else
			printf ("Case #%d: %s\n", i+1, result);
	}

	return 0;
}
