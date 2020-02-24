#include <stdio.h>
int main()
{
	int tcase, size, i, j, index = 1;

	scanf("%d",&tcase);

	while(tcase--)
	{
		scanf("%d",&size);

		float n[size], k[size], temp;
		int indecietful = 0 , inwar = 0;

		for (i = 0; i < size; i++)
			scanf("%f", &n[i]);
		
		for (i = 0; i < size; i++)
			scanf("%f", &k[i]);
		
		for ( i = 0 ; i < size -1  ; i++ )
		{
			for ( j = 0 ; j < size -1 - i ; j++ )
			{
				if ( n[j] < n[j + 1] )
				{
					temp = n[j] ;
					n[j] = n[j + 1] ;
					n[j + 1] = temp ;
				}

				if ( k[j] < k[j + 1] )
				{
					temp = k[j] ;
					k[j] = k[j + 1] ;
					k[j + 1] = temp ;
				}
			}
		}

		i=0;
		j=0;
		while(i<size)
		{
			while(n[i] > k[j] && i < size)
			{
				
				inwar++;
				i++;
				
			}
			j++;
			i++;
		}
		i=0;
		j=0;
		while(j<size)
		{
			while(k[j] > n[i] && j < size)
			{
				
				j++;
			}
			if (k[j] < n[i] && j < size)
			{
				
				indecietful++;
				i++;
				j++;
			}
		}


		printf("Case #%d: %d %d\n",index, indecietful, inwar);

		
		index ++;

	}
}