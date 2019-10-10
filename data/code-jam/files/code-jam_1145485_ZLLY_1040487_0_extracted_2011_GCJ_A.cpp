#include <iostream>
using namespace std;
__int64 gcd(__int64 a, __int64 b){ return a == 0 ? b : gcd(b % a, a); } 
int main()
{
	__int64 N,PD,PG;
	__int64 Cas=0;
	__int64 n,GCD;
	freopen("A_in.in","r",stdin);
	freopen("A_out.out","w",stdout);
	scanf("%I64d",&n);
	while(n--)
	{
		scanf("%I64d%I64d%I64d",&N,&PD,&PG);
		printf("Case #%d: ",++Cas);
		if(PG==100)
		{
			if(PD==100) printf("Possible\n");
			else printf("Broken\n");
			continue;
		}
		if(PG==0)
		{
			if(PD==0) printf("Possible\n");
			else printf("Broken\n");
			continue;
		}
		if(PD==0)
		{
			printf("Possible\n");
			continue;
		}
		GCD=gcd(100,PD);
		if((100/GCD)<=N)  printf("Possible\n");
		else printf("Broken\n");
	}
	return 0;
}