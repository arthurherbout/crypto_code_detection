#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>
#include <cstdio>
#include <cassert>

using namespace std;

typedef long long ll;
typedef long double ld;

#ifdef WIN32
#define LLD "%I64d"
#else
#define LLD "%lld"
#endif

const int maxn = 21;
const int maxk = (1 << maxn) + 5;

pair<double, double> ans[maxk];
int x[maxn * 2];
char s[maxn * 2];

int main()
{
	int NT = 0;
	scanf("%d", &NT);
	for (int T = 1; T <= NT; T++)
	{
		printf("Case #%d: ", T);
		
		scanf("%s", s);
		int n = strlen(s);
		int init = 0;
		int kv = n;
		for (int i = 0; i < n; i++) if (s[i] == 'X')
		{
			kv--;
			init |= (1 << i);
		}
// 		cout << "now " << n << endl;
		int km = 1 << n;
		for (int mask = 0; mask < km; mask++) ans[mask].first = 0;
		ans[init] = make_pair(1, 0);
		double answer = 0;
		for (int mask = 0; mask < km - 1; mask++)
		{
// 			cout << mask << ' ' << ans[mask].first << ' ' << ans[mask].second << endl;
			for (int i = 0; i < n; i++) x[i] = (mask >> i) & 1;
			for (int i = 0; i < n; i++) x[i + n] = x[i];
			for (int i = 0; i < n; i++)
			{
				int cur = i;
				while (x[cur]) cur++;
				int wh = cur % n;
				ans[mask | (1 << wh)].first += ans[mask].first / n;
// 				ans[mask | (1 << wh)].second += ((double)(cur - i) / n) * ans[mask].first + ans[mask].second;
				answer += ans[mask].first / n * (cur - i);
			}
		}
		
// 		cout << km - 1 << ' ' << ans[km - 1].first << ' ' << ans[km - 1].second << endl;
		
		printf("%.10lf\n", n * kv - answer);
		
		fprintf(stderr, "%d/%d cases done!\n", T, NT);
	}
	return 0;
}
