#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

const long long MOD = 1000000007;
const long double EPS = 1e-8;

void solve()
{
	int n; cin >> n;
	vector<string> mas(n);
	for (int i = 0; i < n; i++)
		cin >> mas[i];

	vector<int> p(n);
	for (int i = 0; i < n; i++)
		p[i] = i;

	int amount = 1;
	for (int i = 2; i <= n; i++)
		amount *= i;

	long long ans = 0;
	for (int per = 0; per < amount; per++)
	{
		string s;
		for (int i = 0; i < n; i++)
			s += mas[p[i]];

		bool vgood = true;
		for (char c = 'a'; c <= 'z' && vgood; c++)
		{
			bool good = true;
			bool find = false;
			for (int i = 0; i < s.size(); i++)
			{
				if (s[i] == c)
				{
					if (!find)
						find = true;
					else
						if (i > 0 && s[i - 1] != c)
						{
							vgood = false;
							break;
						}
				}				
			}
		}

		if (vgood)
			ans = (ans + 1) % MOD;

		next_permutation(p.begin(), p.end());
	}

	cout << ans;
}

int main(int argc, char* argv[])
{
	FILE* fin = NULL;
	FILE* fout = NULL;

	fin = freopen("input.txt", "r", stdin);
	fout = freopen("output.txt", "w", stdout);

	int T; cin >> T;
	for (int test = 1; test <= T; test++)
	{
		printf("Case #%d: ", test);
		solve();
		cout << endl;
	}

	if (fin) fclose(fin);
	if (fout) fclose(fout);

	return 0;
}