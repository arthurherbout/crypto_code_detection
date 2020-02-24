#include <bits/stdc++.h>
using namespace std;
char buf[100];
int main()
{
	int test , n , l;
	scanf("%d" , &test);
	for(int tt = 1 ; tt <= test ; tt++)
	{
		scanf("%d%d" , &n , &l);
		bool impossi = false;
		for(int i = 1 ; i <= n ; i++)
		{
			scanf("%s" , buf + 1);
			bool allone = true;
			for(int j = 1 ; j <= l ; j++)
				if(buf[j] == '0')allone = false;
			if(allone)impossi = true;
		}
		scanf("%s" , buf + 1);
		printf("Case #%d: " , tt);
		if(impossi){printf("IMPOSSIBLE\n");continue;}
		for(int i = 1 ; i <= l - 1 ; i++)
			printf("?");
		printf("0");

		printf(" ");
		
		printf("10?");
		for(int i = 1 ; i <= l - 1 ; i++)
			printf("10");
		printf("\n");
	}
	return 0;
}