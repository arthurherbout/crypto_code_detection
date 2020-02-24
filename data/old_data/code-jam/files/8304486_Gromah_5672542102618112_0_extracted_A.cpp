#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;
#define N 100000

const int Pow[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
int Case, n, cnt, len, Go[N], Log[N];
bool Flag[N];
char s[9];
vector <int> Vec[6][N];

void Prepare()
{
	for (int i = 1; i < N; i ++)
		for (Log[i] = 0; i >= Pow[Log[i]] * 10; Log[i] ++) ;
	for (int d = 0; d < 5; d ++)
		for (int i = 1; i < Pow[d + 1]; i ++)
		{
			Go[i] = 0;
			for (int x = i; x; x /= 10)
			{
				int t = x % 10;
				if (t > d + 1)
				{
					Go[i] = N;
					break ;
				}
				if (!t) continue ;
				Go[i] += Pow[d - t + 1];
			}
			if (Go[i] < N) Vec[d][Go[i]].push_back(i);
		}
}

void dfs(int x, int d)
{
	if (Flag[x] || !x) return ;
	cnt ++, Flag[x] = 1;
	for (int i = 0; i < Vec[d][x].size(); i ++)
		dfs(Vec[d][x][i], d);
}

int main()
{
	Prepare();
	scanf("%d", &Case);
	for (int Test = 1; Test <= Case; Test ++)
	{
		for (int i = 1; i < N; i ++) Flag[i] = 0;
		scanf("%s", s);
		cnt = n = 0, len = strlen(s);
		for (int i = 0; i < len; i ++)
			n = n * 10 + s[i] - '0';
		dfs(n, len - 1);
		printf("Case #%d: %d\n", Test, cnt);
	}
	return 0;
}
