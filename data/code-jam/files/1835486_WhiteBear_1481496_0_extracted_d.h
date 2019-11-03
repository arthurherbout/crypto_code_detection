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

#define fill(ar,val) memset((ar),(val),sizeof (ar))

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

#define N 5555
#define K 34

int n, m;
int A[N];
char S[N];

int main()
{
	freopen("d.in", "rt", stdin);
#ifdef XDEBUG	
#else
	freopen("d.in", "rt", stdin);
	freopen("d.out", "wt", stdout);
#endif

	int i, j, k;
	char c;
	int a, d;

	int w[26];
	fill(w,255);
	w['o'-'a'] = 26 + 0;
	w['i'-'a'] = 26 + 1;
	w['e'-'a'] = 26 + 2;
	w['a'-'a'] = 26 + 3;
	w['s'-'a'] = 26 + 4;
	w['t'-'a'] = 26 + 5;
	w['b'-'a'] = 26 + 6;
	w['g'-'a'] = 26 + 7;
	
	int ts;	
#if 1
	int tss;
	in(d, tss);
	rep(ts, 1, tss + 1)
#else
	for(ts = 1; in(d, n) > 0; ++ts)
#endif
	{		
		scanf("%d %s", &k, S);
		int n = strlen(S);
		ll res = 0;
		if(k == 2)
		{
			int q[K][K] = {0};
			for(i = 1; i < n; i++)
			{
				int a = S[i] - 'a';
				int b = S[i - 1] - 'a';
				q[a][b] = 1;
				if(w[a] >= 0) q[w[a]][b] = 1;
				if(w[b] >= 0) q[a][w[b]] = 1;
				if(w[a] >= 0 && w[b] >= 0) q[w[a]][w[b]] = 1;
			}
			int in[K] = {0},out[K] = {0};
			for(i = 0; i < K; i++)
				for(j = 0; j < K; j++)
				{
					in[i] += q[i][j];
					out[j] += q[i][j];
				}
				int dif = 0;
				for(i = 0; i < K; i++)
				{
					dif += in[i] != out[i];
					res += max(in[i],out[i]);
				}
				res += dif == 0;
		}		

		printf("Case #%d: ", ts);
		printf("%lld\n", res);

		cerr << ts << endl;
	}

	return 0;
}
