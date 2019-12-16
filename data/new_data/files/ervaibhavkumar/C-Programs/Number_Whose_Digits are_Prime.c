#include <stdio.h>
int isPrime(int n)
{
	int flag = 1;
	if(n>1)
	{
		for(int i=2;i<n;i++)
		{
			if(n%i==0)
			{
				flag = 0;
				break;
			}
		}
	}
	else
	{
		return 0;
	}
	if(flag)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int isDigitsPrime(int n)
{
	int flag = 0;
	while(n>0)
	{
		int rem = n%10;
		if(isPrime(rem))
		{
			n = n / 10;
			continue;
		}
		else
		{
			flag=1;
			break;
		}
	}
	if(flag==0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main()
{
	int t = 2;
	int n;
  printf("Enter the number till which you want number whose digits are prime : ");
	scanf("%d",&n);
  printf("Numbers are : ");
	while(t<=n)
	{
		if(isDigitsPrime(t))
		{
			printf("%d\n",t);
		}
		t++;
	}
    return 0;
}
