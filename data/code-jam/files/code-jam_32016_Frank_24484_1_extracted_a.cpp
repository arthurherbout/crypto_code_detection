#include <cstdio>
#include<stdlib.h>
#include<cmath>
using namespace std;

#define DEBUG 0
__int64 A[1000], B[1000], n;

void readin(void)
{
	scanf("%d\n", &n);
	for(int i=0;i<n;i++)
		scanf("%lld", &A[i]);

	for(int i=0;i<n;i++)
		scanf("%lld", &B[i]);
}

int compare1 (const void * a, const void * b)
{
  return ( *(__int64*)a - *(__int64*)b );
}

int compare2 (const void * a, const void * b)
{
  return ( *(__int64*)b - *(__int64*)a );
}

int main()
{
	freopen("A-large.in","r",stdin);

#if !DEBUG
	freopen ("A-large.out","w",stdout);
#endif

	int cases;
	__int64 ans;

	scanf("%d\n", &cases);

	for(int i=1;i<=cases;i++)
	{
		readin();
		qsort (A, n, sizeof(__int64), compare1);
		qsort (B, n, sizeof(__int64), compare2);

		ans=0;
		for(int j=0;j<n;j++)
			ans+=A[j]*B[j];
		printf("Case #%d: %lld\n", i,ans);

	
	}
	//

#if !DEBUG
	fclose (stdout);
#endif

	return 0;
}