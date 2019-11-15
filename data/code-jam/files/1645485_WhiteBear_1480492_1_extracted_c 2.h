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

#define N 55
#define M 1012

struct car
{
	bool l;
	double p;
	int s;
};

int n, m;
int li = -1;
car A[N];

void mv(double t)
{
	int i;
	rep(i, 0, n) A[i].p += A[i].s * t;
}

double inter(int i, int j, double mxt)
{
	if(A[i].s == A[j].s) return -1;
	bool b = 0;
	if(A[i].s < A[j].s) swap(i, j), b = 1;
	if(A[i].p > A[j].p - 5) return -1;
	double t = (A[j].p - A[i].p - 5) / (A[i].s - A[j].s);
	if(t > mxt + eps) return -1;
	/*int k;
	rep(k, 0, n) if(k != j && A[k].l != A[i].l && A[k].p + t * A[k].s + 5 > A[i].p + t * A[i].s && A[k].p + t * A[k].s < A[i].p + t * A[i].s + 5) break;
	if(k == n)
	{
		mv(t);
		A[i].l ^= 1;
		return t;
	}
	return inter(k, i, t);*/
	int k;
	rep(k, 0, n) if(k != i && A[k].l != A[j].l && A[j].p + t * A[j].s + 5 > A[k].p + t * A[k].s && A[j].p + t * A[j].s < A[k].p + t * A[k].s + 5) break;
	if(k < n) return -1;
	if(li == j && t < eps) return -1;
	mv(t);
	A[j].l ^= 1;
	li = j;
	return t;
}

int main()
{
	freopen("c.in", "rt", stdin);
	//freopen("in.txt", "rt", stdin);
#ifdef XDEBUG	
#else
	freopen("c.out", "wt", stdout);
#endif

	int i, j, k, kk;
	char c;
	int a, d;
	string s;
	
	int ts;	
#if 1
	int tss;
	in(d, tss);
	rep(ts, 1, tss + 1)
#else
	for(ts = 1; in(d, n) > 0; ++ts)
#endif
	{
		in(d, n);
		rep(i, 0, n)
		{
			cin >> s;
			A[i].l = s == "L";
			in(d, A[i].s);
			in(lf, A[i].p);
		}

		double res = 0., ress = 0.;
		for(;;)
		{
			double t = inf;
			rep(i, 0, n) rep(j, 0, n) if(A[i].s > A[j].s && A[i].l == A[j].l && A[i].p < A[j].p && (A[j].p - A[i].p - 5) / (A[i].s - A[j].s) < t)
				t = (A[j].p - A[i].p - 5) / (A[i].s - A[j].s), k = i, kk = j;
			maxx(ress, res + t);
			if(t == inf) break;
			rep(i, 0, n) if(A[i].l != A[k].l && A[i].p + t * A[i].s > A[k].p + t * A[k].s && A[i].p + t * A[i].s < A[k].p + t * A[k].s + 5) break;
			if(i < n) break;
			rep(i, 0, n) if(A[i].l != A[k].l && A[i].p + t * A[i].s > A[k].p + t * A[k].s && A[i].p + t * A[i].s <= A[k].p + t * A[k].s + 5 + eps && A[i].s < A[k].s) break;
			if(i < n) break;
			rep(i, 0, n) if(A[i].l != A[k].l && A[i].p + t * A[i].s + 5 > A[k].p + t * A[k].s && A[i].p + t * A[i].s < A[k].p + t * A[k].s + 5) break;
			if(i == n)
			{
				if(t < eps && li == k) break;
				res += t;
				mv(t);
				A[k].l ^= 1;
				li = k;
				continue;
			}
			double r = inter(k, i, t);
			if(r < 0.)
			{
				r = inter(k, kk, t);
				if(r < 0.) break;
			}
			res += r;
		}
		maxx(res, ress);

		printf("Case #%d: ", ts);
		if(res >= inf) out(s, "Possible");
		else out(.6lf, res); nl;

		cerr << ts << endl;
	}

	return 0;
}
