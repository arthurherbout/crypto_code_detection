#include<stdio.h>
#include<math.h>
#include<stdlib.h>

	int N;
	long long int L, H, lcm, gcd, f[10010];

long long int GCD(long long int a, long long int b)
{
    while( 1 )
    {
        a = a % b;
		if( a == 0 )
			return b;
		b = b % a;

        if( b == 0 )
			return a;
    }
}

   long long int LCM2 (long long int a, long long int b)
    {
        int i=a*b;
        if(i)  while (b^=a^=b^=a%=b); return i/a;
    }

long long int LCM (long long int a,long long int b)
    {   
		 return a * b / GCD(a, b);

        //int i=0;
        //if (a&&b) while (++i%a || i%b);
        //return i;
    }

int compare (const void * a, const void * b)
{
  return ( *(long long int*)a - *(long long int*)b );
}

bool isrange()
{

	for(int i = 0; i < N; i++)
		if( f[i] <= H && f[i] >= L )
		{
			printf("%lld\n", f[i]);

			return true;
		}
	return false;
}
void solve()
{
	scanf("%d %lld %lld", &N, &L, &H);

	for(int i = 0; i < N; i++)
	{
		scanf("%lld", &f[i]);
	}

	qsort (f, N, sizeof(long long int), compare);

	lcm = 1;
	for(int i = 0; i < N; i++)
	{
		lcm = LCM( f[i], lcm );	
	}

	if( lcm == f[N-1])
	{
		for(long long int i = L; i <=H; i++)
			if( lcm % i == 0 || i %lcm == 0 )
			{
			printf("%lld\n", i);
			return;
			}
		printf("NO\n");
	}
	else
	{

		for(long long int i = L; i <=H; i++)
			if( i %lcm == 0 )
			{
			printf("%lld\n", i);
			return;
			}

			printf("NO\n");
	}


	


}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("data.txt","r",stdin);
	freopen("out.txt","w",stdout);
#endif

	int T;
	scanf("%d\n", &T);
	for(int i = 1; i <= T; i++)
	{
		printf("Case #%d: ", i);
		solve();
	}

	return 0;

}
