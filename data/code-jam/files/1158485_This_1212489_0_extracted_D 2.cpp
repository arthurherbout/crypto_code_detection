/*
 * 2011-06-11  Martin  <Martin@Martin-desktop>

 * 
 */
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>

using namespace std;

#define ll long long

int L;
ll Ans;
bool Found;
char St[200];

void DFS(int Step, ll N)
{
	if (Step == L)
	{
		ll tmp = sqrt((double) N) + 0.5;
		if (tmp * tmp == N)
		{
			Found = 1;
			Ans = N;
		}
		return;
	}
	if (Found)
		return;
	if (St[Step] != '0')
		DFS(Step + 1, N ^ (1LL << (L - Step - 1)));
	if (St[Step] != '1')
		DFS(Step + 1, N);
}

inline void Solve()
{
	scanf("%s", St);
	L = strlen(St);
	Found = 0;
	DFS(0, 0);
	for (int i = L - 1; i >= 0; -- i)
		printf("%d", ((Ans & (1LL << i)) > 0));
	printf("\n");
}

int main()
{
	int TestCase;
	scanf("%d", &TestCase);
	for (int i = 1; i <= TestCase; ++ i)
	{
		printf("Case #%d: ", i);
		Solve();
	}
	return 0;
}
