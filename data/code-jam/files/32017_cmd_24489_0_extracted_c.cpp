#include <iostream>
#include <vector>
#include <string>
using namespace std;

int d[5001];
int u[5001];
bool c[5001];

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int t; cin >> t;
	for(int z = 1; z <= t; ++z)
	{
		int k, n;
		scanf("%d%d", &k, &n);
		for(int i = 0; i < n; ++i)
			scanf("%d", &d[i]);
		memset(u, -1, sizeof u);
		memset(c, 0, sizeof c);
		int sz = 0;
		int cnt = 0;
		int i = 0;
		while(sz < k)
		{
			if(u[i] == -1 && !c[cnt])
			{
				u[i] = cnt;
				c[cnt] = true;
				cnt = 0;
				
				sz++;
			}
			if(u[i] == -1) cnt++;
			i++;			
			if(i == k) i = 0;
		}

//		cout << "Case #" << z << ":";
		printf("Case #%d:", z);
		for(int i = 0; i < n; ++i)
			printf(" %d", u[d[i] - 1] + 1);
		printf("\n");
	}

	return 0;	
}