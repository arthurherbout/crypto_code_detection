#include<stdio.h>
#include<string.h>
#include<malloc.h>

long long int toDeci(char *str, int base);
long long int isPrime(long long int number);

int main()
{	
	int base = 0;
	int cases;
	char str[17];
	scanf("%d" , &cases);  
	long long int deci;
	int cnt =0;
	int i = 0;
	int k = 15;
	int perm = 1;
	long long int p[11];
	p[0] = -1;
	p[1] = -1;
	str[0] = '1';
	str[15] = '1';
	for(i =1;i<15;i++)
			str[i] = '0';	
	str[16] = '\0';
 
	printf("Case #1:\n");
	while(cnt < 50)
	{
		
		for(base = 2;base<11;base++)
		{
			deci = toDeci(str, base);
			
			p[base] = isPrime(deci);

			if(p[base] == -1) 
				break;
		}

		if(base == 11)
		{
			
			printf("%s ", str);

			for(base = 2;base<11;base++)
				printf("%lld " , p[base]);
			printf("\n");
			cnt++;
		}
		

		switch(perm)
		{
			case 1:
				k--;
				if(k > 0)
				{
					str[k] = '1';
					break;
				}
				else
					k = 15;
					perm = 2;
				//break;
			case 2:
				k--;
				if(k > 1)
					str[k] = '0';
				else
					perm = 4;
				break;
			case 4:
				k+=2;
				if(k < 14)
				{
					str[k] = '1';
					break;
}
				else
					k = 14;
					perm = 3;
				
				//break;
			case 3:
				k-=2;
				if(k > 3)
				{	str[k] = '1';
					break;
				}
				else
				{
					k = 2;
					perm = 5;
				}
				//break;
			
			case 5:
				k+=3;
				if(k < 13)
				{
					str[k] = '0';
					break;
				}
				else
				{	for(i =1;i<15;i++)
						str[i] = '0';
					k = 0;
					perm = 6;
				}

			case 6:
				k+=3;
				if(k < 14)
				{
					str[k] = '1';
					break;
}
				else
					k = 14;
					perm = 3;
			
		}
}
	    return 0;
}

long long int toDeci(char *str, int base)
{
    int len = strlen(str);
    long long int power = 1; // Initialize power of base
    long long int num = 0;  // Initialize result
    long long int i;
 
    for (i = len - 1; i >= 0; i--)
    {
	if(str[i] != '0')
        	num += power;

        power = power * base;
    }
 
    return num;
}
 
long long int isPrime(long long int number) 
{
   long long int i = 0;
    
    for (i=2; i*i<=number; i++) 
    {
        if (number % i == 0)  
		return i;
    }
   return -1;
}
