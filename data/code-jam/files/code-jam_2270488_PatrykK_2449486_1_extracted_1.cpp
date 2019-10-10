#include <cstdio>
#define INF 1000
void prep(int n, int m, int r[], int c[])
{
	for(int i=0; i<n; i++) r[i]=0;
	for(int i=0; i<m; i++) c[i]=0;
}
int main()
{
	int d, n, m;
	int t[101][101], r[101], c[101];
	scanf("%d", &d);
	for(int k=1; k<=d; k++)
	{
		scanf("%d %d", &n, &m);
		printf("Case #%d: ", k);
		bool res=true;
		prep(n, m, r, c);
		for(int i=0; i<n; i++)
		{
			for(int j=0; j<m; j++)
			{
				scanf("%d", &t[i][j]);
				r[i] = (r[i] < t[i][j]) ? t[i][j] : r[i];
			}
		}
		for(int j=0; j<m; j++)
		{
			for(int i=0; i<n; i++)
				c[j] = (c[j] < t[i][j]) ? t[i][j] : c[j];
		}
		for(int i=0; i<n; i++)
		{
			for(int j=0; j<m; j++)
				if(t[i][j]<c[j] && t[i][j]<r[i]) { res=false; break; }
			if(!res) break;
		}
		if(res) printf("YES\n");
		else printf("NO\n");
	}	
	return 0;
}
