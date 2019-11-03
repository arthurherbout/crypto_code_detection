#include<stdio.h>
#include<math.h>
#include<stdlib.h>

int compare (const void * a, const void * b)
{
  return ( *(long long int*)a - *(long long int*)b );
}
void solve()
{
	int L, N, C, table[1001];
	long long int t, temp[1001];
	
	scanf("%d %lld %d %d", &L, &t, &N, &C);
	for(int i = 0; i < C; i++)
		scanf("%d", &table[i]);


	int i=0,j=0,index=0,size=0;
	long long int total=0,key=0;
		bool done =false;
		for(i=0;i<N;i++)
		{
			total +=2*table[i];
			if(!done&&total>=t)
			{
				index = i;
				done =true;
				key=(total-t)/2;
			}
		}
		if(N-index>0)
		{
			for(i=index+1;i<N;i++)
			{
				temp[i-index-1] = table[i];
			}
			temp[N-index-1] =key;
			qsort (temp, N-index +1, sizeof(long long int), compare);
			j=N-index-1;
			while(j>=0&&L>0)
			{
				total-=temp[j];
				L--;
				j--;
				
			}
		}

		printf("%lld\n", total);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("data.txt","r",stdin);
	//freopen("out.txt","w",stdout);
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
