#include <stdio.h>

int main()
{
	int n;
	printf("Enter a number : ");
	scanf("%d",&n);
	int num = n;
	int count = 0;
	while((n&1) != 1)
	{
		count++;
		n = n>>1;
	}
	printf("Trailing zeroes in %d are %d\n",num,count);
	count = 0;
	n = num;
	while(n)
	{
		count++;
		n = n>>1;
	}
	printf("Leading zeroes in 32 bit number %d are %d\n",num,32-count);
    return 0;
}
