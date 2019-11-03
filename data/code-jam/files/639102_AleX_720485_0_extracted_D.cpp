#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <set>
#include <cstring>

using namespace std;

#define forn(i, n) for (int i = 0; i < n; i++)
#define seta(a, b) memset(a, b, sizeof(a))
#define fs first
#define sc second
#define y1 botva
#define pb push_back
#define mp make_pair

int const p = 1000000007;
int b, now, n, ans, a[101], aa[101];
bool used[20];

void go(int sum, int now, int max)
{
	if (sum == 0)
	{
	//	return;
		int N = n;
		while (N > 0)
		{
			seta(used, 0);
			memcpy(aa, a, sizeof(aa));
			forn(i, now)
				{
					if (used[aa[i] % b] && aa[i] > 0) return;
					used[aa[i] % b] = true;
					aa[i] /= b;
				}
			N /= b;
		}
		ans++;
//		forn(i, now)
	//		cout << a[i] << " ";
		//cout << endl;
		return;
	}
	for (int w = max; w <= sum; w++)
	{
		a[now] = w;
		go(sum - w, now + 1, w);
	}
}

void solve()
{
	cin >> n >> b;

	ans = 0;
	go(n, 0, 1);
	cout << ans;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	
	int tests;
	cin >> tests;
	forn(t, tests)
	{
		cout << "Case #" << t + 1 << ": ";
		solve();
		cout << endl;
	}

	return 0;
}
