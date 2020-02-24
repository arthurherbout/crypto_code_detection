#include<stdio.h>
#include<malloc.h>

int main()
{	
	int cases;
	scanf("%d" , &cases);
	int N;
	int *arr = (int*)malloc(sizeof(int)*10);
	int i;
	int j;
	int n;
	int cnt = 0;
	int _cases = 0;
	int arr_full = 0;
	while(cases)
	{
		_cases++;
		scanf("%d" ,&N);
		
		for(i = 0;i<10;i++)
			arr[i] = 0;
		cnt = 0;
		arr_full = 0;

		if(N == 0)
			printf("Case #%d: %s\n" , _cases , "INSOMNIA");
		else
		{
			while(arr_full == 0)
			{
				cnt++;
				n = N*cnt;
				while(n != 0)
				{
					j = n%10;
					arr[j] = 1;
					n = n/10;
				}

				arr_full = 1;

				for(i = 0;i<10;i++)
				{
					if(arr[i] == 0)
						arr_full = 0;
				}
			}
			printf("Case #%d: %d\n" , _cases , N*cnt);
	
			/*for(i = 0;i<10;i++)
				printf("%d" , arr[i]);*/
		}
		cases--;
	}
	return 0;
}
