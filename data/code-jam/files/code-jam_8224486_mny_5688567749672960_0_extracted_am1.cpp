#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int flip(int n)
{
	int flip_n = 0;
	while(n!=0)
	{
		flip_n = flip_n*10 + n%10;
		n = n/10;
	}
	return flip_n;
}

int main() {
    
    int t, t1, n;
    scanf("%d", &t);
    t1 = t;
    while ( t-- )
    {
    	int count = 0;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        {
        	count ++;
        	if((flip(i) > i) && (flip(i) < n))
        	{
        		i = flip(i);
        		i =i-1;
			}
        	
		}
        printf("Case #%d: %d\n", (t1-t), count);
    }
    return 0;
}

