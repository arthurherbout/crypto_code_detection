#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <queue>

using namespace std;
typedef long long ll;
typedef long double ld;
const int MAXN = 10100;
const ll MOD = 1e9 + 7;

ifstream fin ("B.in");
ofstream fout ("B.out");

int N, K;
int res[MAXN][MAXN];
ll ps[MAXN];

ll cpow (ll b, ll e)
{
	ll pow = b, res = 1;
	while (e)
	{
		if (e % 2 == 1)
			res = (res * pow) % MOD;
		e /= 2;
		pow = (pow * pow) % MOD;
	}
	return res;
}

ll inv (ll x)
{
	return cpow (x, MOD - 2);
}

int main()
{
	for (int i = 0; i < MAXN; i++)
	{
		for (int j = 0; j < MAXN; j++)
			res[i][j] = 0;
		ps[i] = 0;
	}
	
	res[1][1] = 1;
	ps[1] = 1;
	for (int i = 2; i < MAXN; i++)
	{
		ll cval = inv (i - 1);
		
		for (int j = 1; j <= i; j++)
		{
			ll tot = ps[j] + ps[i-j+1] - res[i-1][j] - res[i-1][j-1];
			tot = (tot + 10 * MOD) % MOD;
			
			tot = (tot * cval) % MOD;
			res[i][j] = tot;
		}
		
		for (int j = 1; j <= i; j++)
		{
			ps[j] = (ps[j] + res[i][j]) % MOD;
		}
	}
	
	int ntest; fin >> ntest;
	for (int test = 0; test < ntest; test++)
	{
	
	fin >> N >> K;
	fout << "Case #" << test + 1 << ": ";
	fout << (MOD + 1 - res[N][K]) % MOD << "\n";
	
	}
	
	return 0;
}
