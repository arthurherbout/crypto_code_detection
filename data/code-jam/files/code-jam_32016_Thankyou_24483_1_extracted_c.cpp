#include <stdio.h>
#include <string.h>
#include <algorithm>
using namespace std;

const int MOD = 1000;

struct node
{
	int arr[2][2];	
};

void check(int &a)
{
	a %= MOD;
	if (a < 0)
		a += MOD;	
}

node matrix_mul(node kk, node tt)
{
	node res;
	for (int i=0; i<2; i++)
		for (int j=0; j<2; j++)
		{
			res.arr[i][j] = 0;
			for (int k=0; k<2; k++)
				res.arr[i][j] += kk.arr[i][k]*tt.arr[k][j];
			check(res.arr[i][j]);
		}	
	return res;
}

node matrix_pow(node tmp, int n)
{
	if (n == 0)
	{
		node res;
		res.arr[0][0] = res.arr[1][1] = 1;
		res.arr[0][1] = res.arr[1][0] = 0;
		return res;	
	}	
	node k = matrix_pow(tmp, n/2);
	node t = matrix_mul(k, k);
	if (n%2 == 1)
		return matrix_mul(t, tmp);
	else
		return t;
}

int main()
{
	//freopen("C-large.in", "r", stdin);
	//freopen("C-large.out", "w", stdout);
	
	node tmp;
	tmp.arr[0][0] = 6;
	tmp.arr[0][1] = 1;
	tmp.arr[1][0] = -4;
	tmp.arr[1][1] = 0;
	int t;
	scanf("%d", &t);
	for (int kth=1; kth<=t; kth++)
	{
		int n;
		scanf("%d", &n);
		printf("Case #%d: ", kth);	
		node res = matrix_pow(tmp, n-1);
		int ans = 6*res.arr[0][0] + 2*res.arr[1][0] - 1;
		check(ans);
		if (ans >= 100)
			printf("%d\n", ans);
		else if (ans >= 10)
			printf("0%d\n", ans);
		else
			printf("00%d\n", ans);	
	}	
	return 0;	
}
