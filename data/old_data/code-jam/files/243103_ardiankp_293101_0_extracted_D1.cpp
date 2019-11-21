#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

typedef long long LL;

int tc, ntc;
LL A, B;

LL proc1(int x)
{
	LL res = x;
	while (x)
	{
		res = res * 10 + (x%10);
		x /= 10;
	}
	return res;
}

LL proc2(int x)
{
	LL res = x;
	x /= 10;
	while (x)
	{
		res = res * 10 + (x%10);
		x /= 10;
	}
	return res;
}

#define MOD 1000000007LL

LL ar[2000005];
LL ar2[2000005];

int main()
{
	FILE* fi = fopen("D-small-attempt3.in", "r");
	FILE* fo = fopen("D-small3.out", "w");

	fscanf(fi, "%d", &ntc);
	int i;
	for (tc=1; tc<=ntc; tc++)
	{
		fscanf(fi, "%I64d %I64d", &A, &B);
		
		int num = 0;
		for (i=1; i<=1000000; i++)
		{
			LL x = proc1( i );
			if (A <= x && x <= B) ar[num++] = x;
			x = proc2( i );
			if (A <= x && x <= B) ar[num++] = x;
		}

		sort(ar, ar+num);		
		for (i=0; i<num; i++)
			ar[i] %= (2 * MOD);

		if (num == 0)
		{
			ar2[0] = B-A+1;
			num = 1;
		}
		else
		{
			ar2[0] = ar[0] - A;
			for (i=1; i<num; i++)
				ar2[i] = ar[i] - ar[i-1];
			ar2[num] = B - ar[num-1] + 1;
			num++;
		}

		for (i=0; i<num; i++) ar2[i] %= (2 * MOD);

		//for (i=0; i<num; i++) printf("%I64d ", ar2[i]); printf("\n");

		LL no = 0, ne = 0;
		LL res = 0;

		res = (ar2[0] * (ar2[0]+1) ) % (2 * MOD);
		for (i=1; i<num; i++)
			(res += (ar2[i] * (ar2[i]-1))) %= (2 * MOD);


		ne = ar2[0];
		ne %= (2 * MOD);
		for (i=1; i<num; i++)
		{
			if (i % 2 == 0)
			{
				(res += ar2[i] * ne * 2) %= (2 * MOD);	
				ne += ar2[i] - 1;
				no++;
				ne %= (2 * MOD);
				no %= (2 * MOD);
			}
			else
			{
				(res += ar2[i] * no * 2) %= (2 * MOD);
				no += ar2[i] - 1;
				ne++;
				ne %= (2 * MOD);
				no %= (2 * MOD);
			}
		}

		res %= (2 * MOD);
		if (res < 0) res += (2 * MOD);
		res /= 2;

		printf("Case #%d: %I64d\n", tc, res);
		fprintf(fo, "Case #%d: %I64d\n", tc, res);
	}

	fclose(fi); fclose(fo);
}