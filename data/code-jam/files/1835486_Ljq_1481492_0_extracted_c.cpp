#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <string>

using namespace std;

struct sNode
{
	long long P, S;
};

bool operator<(const sNode &a, const sNode &b)
{
	return (a.P < b.P)||(a.P == b.P && a.S > b.S);
}

const int MAXN = 210;
const int MAXM = 2000020;

int n, nn;
long long M, F;
long long w[MAXN];
sNode p[MAXN], pp[MAXN];
long long f[MAXM];

long long Work()
{
	cin >> M >> F >> nn;
	for (int i = 0; i < nn; i ++)
	{
		cin >> pp[i].P >> pp[i].S;
		pp[i].S ++;
	}
	sort(pp, pp + nn); 
	long long mx = 0;
	for (int i = 1; i < nn; i ++)
	{
		mx = max(mx, pp[i].S);
		if (pp[i].S <= mx)  pp[i].P = -1;
	}

	n = 0;
	for (int i = 0; i < nn; i ++)
		if (pp[i].P >= 0)
			p[n++] = pp[i];

	w[0] = F + p[0].P * p[0].S;
	for (int i = 1; i < n; i ++)
		w[i] = w[i-1] + p[i].P * (p[i].S - p[i-1].S);

	memset(f, 0xff, sizeof(f));

	f[0] = 0;
	for (int i = 0; i < M; i ++)
	{
		if (f[i] < 0)  continue;
		for (int j = n - 1; j >= 0; j --)
			if (i + w[j] <= M)
				f[i+w[j]] = max(f[i+w[j]], f[i] + p[j].S);
	}

	long long ans = 0;

	for (int i = 0; i <= M; i ++)
		ans = max(ans, f[i]);

	for (int i = max(0LL, M - w[0]); i <= M; i ++)
		if (M - i >= F)
			ans = max(ans, f[i] + (M - i - F) / p[0].P);

	return ans;
}

int main()
{
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("output.txt", "w", stdout);

	cin.sync_with_stdio(false);

	int T;
	cin >> T;
	for (int k = 1; k <= T; k ++)
	{
		cout << "Case #" << k << ": " << Work() << endl;
	}

	return 0;
}