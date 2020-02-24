#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int *findCount(char *str)
{
	int flag;
	static int freq[52] = {0};
	for(int i=0;i<strlen(str);i++)
	{
		if(str[i]>=65 && str[i]<=90)
		{
			freq[((int)str[i])-65+26]++;
		}
		else if(str[i]>=97 && str[i]<=122)
		{
			freq[((int)str[i])-97]++;
		}
	}
	return freq;
}
int main()
{
	char *str = (char *)malloc(100 * sizeof(char));
	printf("Enter a string : ");
	scanf("%s",str);
	int *fre = (int *)malloc(26 * sizeof(int));
	fre = findCount(str);
	printf("Frequency of all characters is : ")
	for(int i=0;i<52;i++)
	{
		if(fre[i]!=0)
		{
			if(i>=0 && i<=25)
			{
				printf("%c %d\n",(i+97),fre[i]);
			}
			else
			{
				printf("%c %d\n",(i+65-26),fre[i]);	
			}
		}
	}
    return 0;
}
