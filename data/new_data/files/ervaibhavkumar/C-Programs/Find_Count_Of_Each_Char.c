#include <stdio.h>
void find(char *a)
{
	int n = strlen(a);
	int count = 0;
	int orig[40] = {0};  // An arrya to keep count of each character
	int chk[125][10000] = {0};  // A 2-D array to check whether a character's count has been stored or not
	int flag = 0; // A variable to check whether that a a character went in a iteration or not
	for(int i=0;i<n;i++)
	{
		count = 0;
		flag = 0;
		for(int j=0;j<n;j++)
		{
			if(a[i]==a[j])
			{
				count++;
				if((chk[(int)a[i]][((int)a[i])-((int)'a')])==0)
				{
					orig[((int)a[i])-((int)'a')] = orig[((int)a[i])-((int)'a')]+1;
					flag = 1;	
				}
			}
		}
		chk[(int)a[i]][((int)a[i])-((int)'a')] = 1;
		if(flag==1)
		{
			printf("%c \t %d\n",a[i],orig[((int)a[i])-((int)'a')]);
		}
	}
}
int main(int argc, char const *argv[])
{
	char a[100000];
	gets(a);
	find(a);
	return 0;
}
