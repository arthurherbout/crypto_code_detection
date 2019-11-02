#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;
vector<int> code[2000001];

int main()
{
	int ncas;
	scanf("%d",&ncas);
	for(int t=1;t<=ncas;t++)
	{
		int n;
		int a[201];
		scanf("%d", &n);
		for(int i=0;i<=2000000;i++)
		{
			code[i].clear();
		}
		for(int i=0;i<n;i++)
		{
			scanf("%d",&a[i]);
		}
		printf("Case #%d:",t);
		for(int i=0;i<(1<<n);i++)
		{
			int ii = i;
			int sum = 0;
			int k = 0;
			while(ii)
			{
				if(ii % 2 == 1) sum += a[k];
				ii >>= 1;
				k+=1;
			}
			code[sum].push_back(i);
		}
		
		putchar(10);
		for(int i=0;i<=2000000;i++)
		{
			bool printed = 0;
			if(code[i].size() >= 2)
			{
				int csize = code[i].size();
				for(int j=0;j<csize;j++)
				{
					for(int k=0;k<csize;k++)
					{
						if((code[i][j] & code[i][k]) == 0)
						{
							printed = 1;	
							int ii = code[i][j];
							int l = 0;
							while(ii)
							
							{
								if(ii % 2 == 1)
								{	
									printf("%d",a[l]);
									if(ii == 1) printf("\n");
									else printf(" ");
								}

								ii >>= 1;
								l+=1;
							}
							ii = code[i][k];
							l = 0;
							while(ii)
							{
								if(ii % 2 == 1)
								{	
									printf("%d",a[l]);
									if(ii == 1) printf("\n");
									else printf(" ");
								}
								ii >>= 1;
								l+=1;
							}
							break;
						}
					}
					if(printed) break;
				}		
				if(printed) break;
			}
			if(printed) break;
		}
	}
	return 0;
}
