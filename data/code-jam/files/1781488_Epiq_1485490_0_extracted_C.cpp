#include <cstdio>
#include <iostream>
#include <cstring>
#define Max 100
using namespace std;

int T;
int N, M;
long long a[Max];
int A[Max];
long long b[Max];
int B[Max];

long long D[Max + 1][Max + 1];

long long Solve()
{
	memset(D, 0, sizeof(D));
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
		{
			D[i][j] = max(D[i-1][j-1], max(D[i-1][j], D[i][j-1]));
			if (A[i-1] == B[j-1])
			{
				long long t = min(a[i-1], b[j-1]);
				D[i][j] += t;
				a[i-1] -= t;
				b[j-1] -= t;
			}
		}
	}
	
//	for (int i = 1; i <= N; i++){
//		for (int j = 1; j <= M; j++) printf("%I64d ", D[i][j]);
//		printf("\n");
//	}
	return D[N][M];
}

int main()
{
	//freopen("C.in", "r", stdin);
	
	scanf("%d", &T);
	for (int test = 0; test < T; test++)
	{
		scanf("%d %d", &N, &M);
		for (int i = 0; i < N; )
		{
			scanf("%I64d %d", &a[i], &A[i]);
			if (i > 0 && A[i - 1] == A[i])
			{
				a[i - 1] += a[i];
				N--;
			}
			else i++;
		}
		for (int j = 0; j < M; )
		{
			scanf("%I64d %d", &b[j], &B[j]);
			if (j > 0 && B[j - 1] == B[j])
			{
				a[j - 1] += a[j];
				M--;
			}
			else
			j++;
		}
		
		printf("Case #%d: %I64d\n", test + 1, Solve());
	}
	return 0;
}
