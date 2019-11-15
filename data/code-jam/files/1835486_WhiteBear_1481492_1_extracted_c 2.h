#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <sstream>
using namespace std;
#pragma comment(linker, "/STACK:255000000")

typedef long long ll;

#define rep(i, a, b) for(i = (a); i < (b); ++i)
#define repb(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define repd(i, a, b, d) for(i = (a); i < (b); i += (d))
#define repbd(i, a, b, d) for(i = (a) - 1; i >= (b); i -= (d))
#define reps(i, s) for(i = 0; (s)[i]; ++i)
#define repl(i, l) for(i = l.begin(); i != l.end(); ++i)

#define in(f, a) scanf("%"#f, &(a))

bool firstout = 1;

#define out(f, a) printf("%"#f, (a))
#define outf(f, a) printf((firstout) ? "%"#f : " %"#f, (a)), firstout = 0
#define nl printf("\n"), firstout = 1

#define all(x) (x).begin(),(x).end()
#define sqr(x) ((x) * (x))
#define mp make_pair

template<class T>
T &minn(T &a, T b)
{
	if(b < a) a = b;
	return a;
}

template<class T>
T &maxx(T &a, T b)
{
	if(a < b) a = b;
	return a;
}

#define inf 1012345678
#define eps 1e-9


#ifdef XDEBUG
#define mod 23
#else
#define mod 1000000009
#endif

int &madd(int &a, int b)
{
	a += b;
	if(a >= mod) a -= mod;
	return a;
}

int &msub(int &a, int b)
{
	a -= b;
	if(a < 0) a += mod;
	return a;
}

int &mmult(int &a, int b)
{
	return a = (ll)a * b % mod;
}

int mdiv(ll a, ll b, ll m)
{
	a = (a % m + m) % m;
	b = (b % m + m) % m;
	if(a % b == 0) return a / b;
	return (a + m * mdiv(-a, m, b)) / b;
}

#define N 1012
#define M 1012

int n, m;
ll mon, fee;
pair<ll, ll> A[N];

void add(ll &res, ll a)
{
	res += a;
	if(res > (ll)inf * inf) res = (ll)inf * inf;
}

ll mult(ll a, ll b)
{
	if(a > (ll)inf * inf / b) return (ll)inf * inf;
	return a * b;
}

ll cnt(ll x)
{
	ll res = 0;
	int i;
	for(i = 0; i < n; ++i) if(x > A[i].second) add(res, mult(A[i].second - ((i) ? A[i - 1].second : 0), A[i].first));
	else
	{
		add(res, mult(x - ((i) ? A[i - 1].second : 0), A[i].first));
		return res;
	}
	return (ll)inf * inf;
}

ll slv(ll d, ll f)
{
	ll x = d / f;
	return mult(d % f, cnt(x + 1)) + mult(f - d % f, cnt(x)) + mult(fee, f);
}

bool can(ll x)
{
	ll l, r, mid1, mid2, a, b;
	l = 1; r = x + 1;
	for(; l + 1 < r;)
	{
		mid1 = (2 * l + r) / 3;
		mid2 = (l + 2 * r) / 3;
		a = slv(x, mid1);
		b = slv(x, mid2);
		if(a <= mon || b <= mon) return 1;
		if(a < b) r = mid2 - 1;
		else l = mid1 + 1;
	}
	return slv(x, l) <= mon;
}

int main()
{
	freopen("c.in", "rt", stdin);
#ifdef XDEBUG	
#else
	freopen("c.in", "rt", stdin);
	freopen("c.out", "wt", stdout);
#endif

	int i, j, k;
	char c;
	int a, d;
	
	int ts;	
#if 1
	int tss;
	in(d, tss);
	rep(ts, 1, tss + 1)
#else
	for(ts = 1; in(d, n) > 0; ++ts)
#endif
	{
		in(lld, mon); in(lld, fee); in(d, n);
		rep(i, 0, n) in(lld, A[i].first), in(lld, A[i].second), ++A[i].second;
		sort(A, A + n);
		int res = 0;

		ll l = 0, r = mon + 1, mid;
		for(; l + 1 < r;)
		{
			mid = (l + r) / 2;
			if(can(mid)) l = mid;
			else r = mid;
		}

		printf("Case #%d: ", ts);
		out(lld, l); nl;

		cerr << ts << endl;
	}

	return 0;
}
