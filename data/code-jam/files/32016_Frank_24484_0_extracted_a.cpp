#include <cstdio>
#include<stdlib.h>
#include<cmath>
using namespace std;

#define DEBUG 0

int A[1000], B[1000], n;

void readin(void)
{
	scanf("%d\n", &n);
	for(int i=0;i<n;i++)
		scanf("%d", &A[i]);

	for(int i=0;i<n;i++)
		scanf("%d", &B[i]);
}

int compare1 (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int compare2 (const void * a, const void * b)
{
  return ( *(int*)b - *(int*)a );
}

int main()
{
	freopen("A-small-attempt0.in","r",stdin);

#if !DEBUG
	freopen ("A-small-attempt0.out","w",stdout);
#endif

	int cases, ans;

	scanf("%d\n", &cases);

	for(int i=1;i<=cases;i++)
	{
		readin();
		qsort (A, n, sizeof(int), compare1);
		qsort (B, n, sizeof(int), compare2);

		ans=0;
		for(int j=0;j<n;j++)
			ans+=A[j]*B[j];
		printf("Case #%d: %d\n", i,ans);

	
	}
	//

#if !DEBUG
	fclose (stdout);
#endif

	return 0;
}