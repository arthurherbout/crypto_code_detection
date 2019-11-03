#include <cstdio>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstring>
#include <vector>

using namespace std;
char s1[20000];
char s2[2000];
int check(int i)
{
	for (int x = 0; x < i ; ++x)
	{
		if (s1[x]!=s2[x])
		{
			return 0;
		}
	}
	return 1;
} 
int check2(int x)
{
	//int n =strlen(s1);
	for (int i = 0; i < x; ++i)
	{
		if (s1[i]!=s2[i])
		{
			return i;
		}
	}
	return x;
}
int check3(int i)
{
	if (s1[i+1]!='?'&&s2[i+1]!='?'&&abs(s1[i+1]-s2[i+1])>5)
	{
		return 1;
	}
	return 0;
}
int main()
{
	int n ;
	scanf("%d",&n);
	for (int num = 0; num < n; ++num)
	{	
		printf("Case #%d: ",num+1 );
		scanf("%s",s1);
		scanf("%s",s2);
		int len =strlen(s1);
		for (int i = 0; i < len; ++i)
		{	
			int c = check2(i);
			if ((s1[i]=='?'&&s2[i]=='?'&&check3(i)!=1)&&((i==0)||check(i)==1))
			{
				s1[i]='0';
				s2[i]='0';
			}
			else if ((s1[i]=='?'&&s2[i]=='?')&&(c!=i&&s1[c]<s2[c]))
			{
				s1[i]='9';
				s2[i]='0';
			}
			else if ((s1[i]=='?'&&s2[i]=='?')&&(c!=i&&s1[c]>s2[c]))
			{
				s1[i]='0';
				s2[i]='9';
			}
			else if (s1[i]=='?'&&s2[i]!='?'&&check3(i)!=1)
			{
				s1[i]=s2[i];
				//s2[i]='9';
			}
			else if (s1[i]!='?'&&s2[i]=='?'&&check3(i)!=1)
			{
				s2[i]=s1[i];
				//s2[i]='9';
			}
			else if(s1[i]!='?'&&s2[i]=='?'&&check3(i)==1)
			{	
				if (s1[i]=='0'&&s1[i+1]<s2[i+1])
				{
					s2[i]='0';
				}
				else if (s1[i]=='0'&&s1[i+1]>s2[i+1])
				{
					s2[i]='1';
				}
				else if (s1[i]=='9'&&s1[i+1]>s2[i+1])
				{
					s2[i]='9';
				}
				else if (s1[i+1]>s2[i+1])
				{
					//s1[i]='0';
					s2[i]=s1[i]+1;
				}
				else //(s1[i+1]>s2[i+1])
				{
					s2[i]=s1[i]-1;
				}
			}
			else if(s1[i]=='?'&&s2[i]!='?'&&check3(i)==1)
			{	
				if (s2[i]=='0'&&s1[i+1]>s2[i+1])
				{
					s1[i]='0';
				}
				else if (s2[i]=='0'&&s1[i+1]<s2[i+1])
				{
					s1[i]='1';
				}
				else if (s2[i]=='9'&&s1[i+1]<s2[i+1])
				{
					s1[i]='9';
				}
				else if (s1[i+1]>s2[i+1])
				{
					//s1[i]='0';
					s1[i]=s2[i]+1;
				}
				else //(s1[i+1]>s2[i+1])
				{
					s1[i]=s2[i]-1;
				}
			}
			else if(s1[i]=='?'&&s2[i]=='?'&&check3(i)==1)
			{
				if (s1[i+1]>s2[i+1])
				{
					s1[i]='0';
					s2[i]='1';
				}
				else //(s1[i+1]>s2[i+1])
				{
					s1[i]='1';
					s2[i]='0';
				}
			}

		}
		printf("%s ",s1 );
		printf("%s ",s2 );
		printf("\n");
	}


return 0;

}