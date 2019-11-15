#include <stdio.h>
#include <string.h>
#include <algorithm>
#define INF 10000000
using namespace std; 
char eng[110][110]; 
int q[1100], n, m; 
int find(char name[])
{ 
	int l = 0, r = n - 1; 
	while (l <= r)
	{ 
		int mid = (l + r) / 2, tmp = strcmp(name, eng[mid]);
		if (!tmp)
			return mid; 
		else
			if (tmp > 0)
				l = mid + 1; 
			else
				r = mid - 1; 
	}
	return -1; 
}
int cmp(const void *a, const void *b)
{ 
	return strcmp((char *)a, (char *)b); 
}
int f[1100][110]; 
int main() 
{ 
	freopen("A-large.in", "r", stdin); 
	freopen("chfoutal.txt", "w", stdout); 
	int T, i, j, k, tcnt = 0; 
	scanf("%d", &T); 
	while (T--)
	{ 
		scanf("%d\n", &n); 
		for (i = 0; i < n; i++)
			gets(eng[i]); 
		qsort(eng, n, sizeof(eng[0]), cmp); 
		scanf("%d\n", &m); 
		for (i = 0; i < m; i++) 
		{
			char tmp[110]; 
			gets(tmp); 
			q[i] = find(tmp); 
		}
		for (i = 0; i < m; i++) 
			for (j = 0; j < n; j++) 
				f[i][j] = INF; 
		for (j = 0; j < n; j++) 
			f[0][j] = q[0] == j ? INF : 0; 
		for (i = 1; i < m; i++) 
			for (j = 0; j < n; j++) 
				if (j != q[i])
				{
					for (k = 0; k < n; k++)
						f[i][j] <?= f[i - 1][k] + (j != k); 
				}
				else
				{ 
					f[i][j] = INF; 
				}
		/*for (i = 0; i < m; i++) 
		{ 
			for (j = 0; j < n; j++) 
				printf("%d ", f[i][j]); 
			printf("\n"); 
		}*/
		int ans = INF; 
		for (i = 0; i < n; i++) 
			ans <?= f[m - 1][i];
		printf("Case #%d: %d\n", ++tcnt, ans); 
	}
}
