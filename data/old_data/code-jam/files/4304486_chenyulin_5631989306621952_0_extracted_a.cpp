#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;

char s [1005];
int n ;
char out[1005];
void putfirst(char c)
{
	int len = strlen(out);
	for (int i = len; i >0 ; --i)
	{
		out[i]=out[i-1];
	}
	out[0]=c;
	out[len+1]='\0';
	return;
}
void putlater(char c)
{
	int len = strlen(out);
	out[len]=c;
	out[len+1]='\0';
	return;
}
int main()
{
	int tot;
	scanf("%d",&tot);
	for (int i = 0; i < tot; ++i)
	{	
		scanf("%s",s);
		//printf("%s\n", s);
		out[0]=s[0];
		if (s[1]=='\0')
		{
			printf("Case #%d: ",i+1 );
			printf("%c\n",s[0] );
			continue;
		}
		out[1]='\0';
		for (int in = 1; s[in]!='\0' ; ++in)
		{	
			//printf("%d\n", in);
			if (s[in]>=out[0])
			{
				putfirst(s[in]);
			}
			else
			{
				putlater(s[in]);
			}
			//printf("%s\n",out );
		}
		printf("Case #%d: ",i+1 );
		printf("%s\n", out);
	}
return 0;
}