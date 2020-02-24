#include <stdio.h>
#include <limits.h>
int main()
{
	int n;
	printf("Enter number of elements : ");
	scanf("%d",&n);
	int max = INT_MIN , max2 = INT_MIN;
	printf("Enter numbers : \n");
	int tmp;
	for(int i=0;i<n;i++)
	{
		scanf("%d",&tmp);
		if(i==0)
		{
			max2 = tmp;
			max = tmp;
		}
		else if(max<tmp)
		{
			max2 = max;
			max = tmp;
		}
		else if(max2<tmp)
		{
			max2 = tmp;
		}
	}
	printf("Second Largest number is %d\n",max2);
	return 0;
}
