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

struct pt
{
	int x, y;
	pt(){}
	pt(int x, int y): x(x), y(y) {}
};

inline pt operator-(const pt &a, const pt &b)
{
	return pt(a.x - b.x, a.y - b.y);
}

inline int operator*(const pt &a, const pt &b)
{
	return a.x * b.y - a.y * b.x;
}

inline int operator/(const pt &a, const pt &b)
{
	return a.x * b.x + a.y * b.y;
}

inline int sgn(int a)
{
	if (a == 0) return 0;
	return (a > 0 ? 1 : -1);
}

inline bool ison(pt a, pt b, pt c)
{
	return (a - b) * (c - b) == 0 && (a - c) / (b - c) >= 0 && (a - b) / (c - b) >= 0;
}

inline bool intersect(pt a, pt b, pt c, pt d)
{
	if (ison(a, c, d) || ison(b, c, d)) return true;
	if (ison(c, a, b) || ison(d, a, b)) return true;
	return sgn((c - a) * (b - a)) * sgn((d - a) * (b - a)) < 0 &&
			sgn((a - c) * (d - c)) * sgn((b - c) * (d - c)) < 0;
}

const int maxn = 25;

int perm[maxn];
int ans[maxn];
pt p[maxn];
int n;

int main()
{
	int NT = 0;
	scanf("%d", &NT);
	for (int T = 1; T <= NT; T++)
	{
		printf("Case #%d:", T);
		
		scanf("%d", &n);
		for (int i = 0; i < n; i++) scanf("%d%d", &p[i].x, &p[i].y);
		for (int i = 0; i < n; i++) perm[i] = i;
		ll bestsq = 0;
		do
		{
			perm[n] = perm[0];
			ll sq = 0;
			for (int i = 0; i < n; i++) sq += p[perm[i]] * p[perm[i + 1]];
			sq = abs(sq);
			if (sq <= bestsq) continue;
			bool ok = true;
			for (int i = 0; i < n && ok; i++)
			{
				for (int j = i + 2; j < n && ok; j++) if (i + n != j + 1) ok = ok && !intersect(p[perm[i]], p[perm[i + 1]], p[perm[j]], p[perm[j + 1]]);
			}
			if (!ok) continue;
			bestsq = sq;
			for (int i = 0; i < n; i++) ans[i] = perm[i];
		} while (next_permutation(perm, perm + n));
		
		for (int i = 0; i < n; i++) printf(" %d", ans[i]);
		printf("\n");
		
		fprintf(stderr, "%d/%d cases done!\n", T, NT);
	}
	return 0;
}
