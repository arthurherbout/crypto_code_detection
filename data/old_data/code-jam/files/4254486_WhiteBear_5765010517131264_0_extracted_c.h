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
#define mod 1000000007
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

#define N 512
#define M 1012

int n, m, q;
pair<int, int> A[N], AA[N];
double D[N][N][2];

int main()
{
#ifdef XDEBUG
    freopen("in.txt", "rt", stdin);
#else
    freopen("x.in", "rt", stdin);
    freopen("x.out", "wt", stdout);
#endif

    int i, j, k;
    char c;
    int a, d;
    int v;

    int ts;
#if 1
    int tss;
    in(d, tss);
    rep(ts, 1, tss + 1)
#else
    for(ts = 1; in(d, v) > 0; ++ts)
#endif
    {
        in(d, v);
        in(d, n);
        rep (i, 0, n) in(d, AA[i].first);
        rep (i, 0, n) in(d, AA[i].second);
        sort (AA, AA + n);
        m = 0;
        for (i = 0; i < n && AA[i].first < 0; ++i) if (m == 0 || AA[i].second > A[m - 1].second) {
                if (m && AA[i].first == A[m - 1].first) A[m - 1] = AA[i];
                else A[m++] = AA[i];
            }
        A[m++] = mp(0, 0);
        for (; i < n; ++i) {
            for (; A[m - 1].first && AA[i].second >= A[m - 1].second; --m);
            if (AA[i].first > A[m - 1].first) A[m++] = AA[i];
        }
        n = m;
        rep (i, 0, n) rep (j, 0, n) rep (k, 0, 2) D[i][j][k] = inf * 1. * inf * inf;
        rep (i, 0, n) if (A[i].first == 0) break;
        D[i][i][0] = 0.;
        repb (i, n, 0) rep (j, i, n) rep (k, 0, 2) if (D[i][j][k] < inf * 1. * inf * inf - 1) {
                        double t = D[i][j][k];
                        double x = k == 0 ? A[i].first - A[i].second * t : A[j].first + A[j].second * t;
                        int ii = i;
                        int jj = j;
                        if (k) {
                            for (; jj + 1 < n && A[jj + 1].first + A[jj + 1].second * t <= x; ++jj);
                        } else {
                            for (; ii - 1 >= 0 && A[ii - 1].first - A[ii - 1].second * t >= x; --ii);
                        }
                        if (ii == 0 && jj == n - 1) minn(D[ii][jj][k], t);
                        if (ii - 1 >= 0) minn(D[ii - 1][jj][0], t + (x - (A[ii - 1].first - A[ii - 1].second * t)) / (v - A[ii - 1].second));
                        if (jj + 1 < n) minn(D[ii][jj + 1][1], t + (A[jj + 1].first + A[jj + 1].second * t - x) / (v - A[jj + 1].second));
                    }
        double res = min(D[0][n - 1][0], D[0][n - 1][1]);
        printf("Case #%d: ", ts);
        outf(.7lf, res); nl;

#ifndef XDEBUG
        cerr << ts << endl;
#endif
    }

    return 0;
}
