#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include <vector>

#define mp make_pair
#define pb push_back

using namespace std;

typedef long long ll;

const int inf = 0x3f3f3f3f;

const double eps = 1e-8;

int dcmp(double x) { return fabs(x) <= eps ? 0 : (x > 0 ? 1 : -1); }

int tot = 1;
int e[1000001], nxt[1000001], g[10001];
double sqr(double x) { return x * x; }


double a[100001], b[100001], c[100001];

bool vis[10001];
int n;
int q[100001];

void add(int u, int v)
{
	e[tot] = v; nxt[tot] = g[u]; g[u] = tot ++;
}

bool check(double x)
{
	for (int i = 1; i <= n; i ++) g[i] = 0, vis[i] = false;
	tot = 1;
	for (int i = 1; i <= n; i ++)
		for (int j = 1; j <= n; j ++)
			if (i != j)
			{
				double tmp = sqrt(sqr(a[i] - a[j]) + sqr(b[i] - b[j]) + sqr(c[i] - c[j]));
				if (dcmp(tmp - x) <= 0) add(i, j);
			}
	int h = 0, t = 1;
	q[t] = 1; vis[1] = true;
	while (h < t)
	{
		int k = q[++ h];
		for (int i = g[k]; i; i = nxt[i])
			if (!vis[e[i]]) vis[e[i]] = true, q[++ t] = e[i];
	}
	return vis[2] == true;
}

int main( )
{
	int T, tp = 0;
	double x, y, z;
	int m;
	scanf("%d", &T);
	while (T --)
	{
		scanf("%d %d", &n, &m);
		for (int i = 1; i <= n; i ++)
			scanf("%lf %lf %lf %lf %lf %lf", &a[i], &b[i], &c[i], &x, &y, &z);

		int beta = 0.0;
		double l = 0.0, r = 999999.9;
		for (int i = 1; i <= 100; i ++)
		{
			double mid = (l + r) / 2.0;
			if (check(mid)) r = mid;
			else l = mid;
		}
		++ tp;
		printf("Case #%d: %.7f\n", tp, l);
	}
	return 0;
}
