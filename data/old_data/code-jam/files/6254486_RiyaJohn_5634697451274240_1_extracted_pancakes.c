#include<stdio.h>
#include<malloc.h>

void fnc(char s[] , int r);

int main()
{	
	int cases;
	scanf("%d" , &cases);
	char stack[101];
	char* s = (char*)malloc(sizeof(char)*101); 
	int i;
	int j;
	int n;
	int cnt = 0;
	int fnd = 0;
	int _cases = 0;
	while(cases)
	{
		cnt = 0;
		fnd = 0;
		_cases++;
		scanf("%s" , s);
		
	for(i =0;i<100 && s[i] != '\0';i++)
		stack[i] = s[i];
	stack[i] = '\0';
		
		n = i;

		while(fnd == 0)
		{
			
			for(i =0;i<n && stack[i] == '+';i++);
			if(i == n)
			{
				fnd = 1;
			}
			else
			{
				for(j = 0;j<i;j++)
					stack[j] = '-';
				if(j != 0)
					cnt++;
			}
			
			for(i = n-1;i>=0 && stack[i] == '+';i--);

			if(i != -1)				
			{	
				cnt++;
				fnc(stack,i+1);
			}
			

			for(i = 0;i<n && stack[i] == '+';i++);
			
			if(i == n)
				fnd = 1;
		}

			printf("Case #%d: %d\n" ,_cases, cnt);
	
		cases--;		
	}

	return 0;
}

void fnc(char s[] , int r)
{
	char temp[r+1];
	int i;
	int j;
	int k;
	for(i = 0;i<r;i++)
		temp[i] = s[i];
	k = 0;
	for(j = i-1;j>=0;j--)
	{
		s[k] = temp[j];
		k++;
	}	
	for(i = 0;i<k;i++)
	{
		if(s[i] == '+')
			s[i] = '-';
		else
			s[i] = '+';
	}
}
