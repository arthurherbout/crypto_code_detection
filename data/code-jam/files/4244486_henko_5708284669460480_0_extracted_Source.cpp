#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <tuple>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <stack>
#include <utility>
#include <string.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <cctype>

using namespace std;

typedef unsigned int uint;
typedef long long ll;
typedef long lint;

#define MP make_pair
#define PB push_back
#define EPS 0.0000001
#define ALL(a) a.begin(), a.end()

const int INF = 1000 * 1000 * 1000;
const int MOD = 1000 * 1000 * 1000 + 7;
const lint INF_L = 1000 * 1000 * 1000;
const ll INF_LL = 1LL * INF_L * INF_L;
const ll MOD_LL = 1000000000007LL;


ll gcd(ll a, ll b)
{
	while (b)
	{
		a %= b;
		swap(a, b);
	}
	return a;
}

ll lcm(ll a, ll b)
{
	return (a / gcd(a, b)) * b;
}


int k, l, s;
string kb, tar;

int strNum = 0;
int targets = 0;
int bananas = 0;

int countOccurs(string &str)
{
	int cnt = 0;
	for (int i = 0; i <= str.length() - l; ++i)
	{
		cnt += (tar == str.substr(i, l));
	}
	return cnt;
}

void countTargets(string &cur)
{
	if (cur.length() == s)
	{
		int occurs = countOccurs(cur);
		targets += occurs;
		bananas = max(bananas, occurs);
		strNum++;
		return;
	}

	for (int i = 0; i < k; ++i)
	{
		string str = cur + kb[i];
		countTargets(str);
	}
}

//#define ONLINE_JUDGE
int main()
{
#ifndef ONLINE_JUDGE
	freopen("B-small-attempt0 (1).in", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int t;
	cin >> t;
	for (int i = 1; i <= t; ++i)
	{
		cin >> k >> l >> s;
		cin >> kb >> tar;
		vector <bool> u(l, false);
		string str = "";
		strNum = 0;
		targets = 0;
		bananas = 0;
		countTargets(str);
		long double ans = 1.0 * bananas - 1.0 * targets / strNum;
		printf("Case #%d: %.8f\n", i, ans);
	}

	return 0;
}