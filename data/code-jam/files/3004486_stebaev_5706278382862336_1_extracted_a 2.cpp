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

long long gcd(long long a, long long b) {
	if (b > a) swap(a, b);
	while (b) {
		a %= b;
		swap(a, b);
	}
	return a;
}

//#define __int128 long long

void solve()
{
	__int128 a = 0, b = 0;
	string s; cin >> s;
	int i = 0;
	for (i = 0; s[i] != '/'; i++)
		a = 10 * a + s[i] - '0';
	for (i++; i < s.size(); i++)
		b = 10 * b + s[i] - '0';

	if (b & 1)
	{
		cout << "impossible";
		return;
	}
	if ((a == b))
	{
		cout << "0";		
		return;
	}

	for (int i = 1; i <= 40; i++)
	{
		long long l = 0, r = (1 << i);

		while (l + 1 < r)
		{
			long long m = (l + r) / 2;

			__int128 a1 = m*b, a2 = (1 << i)*a;
			if (a1 == a2)
			{				
				for (int j = 0; j <= i; j++)
				{
					m -= 1 << j;
					if (m<=0)
					{
						cout << i - j;
						return;
					}
				}
			}

			if (a1 < a2)
				l = m;
			else r = m;
		}
	}
	
	cout << "impossible";	
}

int main(int argc, char* argv[])
{
	/*
	set<long double> mas;
	mas.insert(0); mas.insert(100);
	for (int i = 0; i <= 40; i++)
	{
		cout << i << ":";
		for (auto p = mas.begin(); p != mas.end(); p++)
			cout << ' '<<*p;
		cout << endl;

		set<long double> new_one;
		for (auto p1 = mas.begin(); p1 != mas.end(); p1++)
			for (auto p2 = mas.begin(); p2 != mas.end(); p2++)
				new_one.insert((*p1 + *p2) / 2);
		for (auto p = new_one.begin(); p != new_one.end(); p++)
			mas.insert(*p);
	}
	return 0;
	*/

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