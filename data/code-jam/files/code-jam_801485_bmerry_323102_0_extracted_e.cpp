#include <string>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include <cstddef>
#include <algorithm>
#include <utility>
#include <iterator>
#include <numeric>
#include <list>
#include <complex>
#include <cstdio>
#include <climits>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<ll> pnt;
typedef pair<int, int> pii;

#define RA(x) (x).begin(), (x).end()
#define FE(i, x) for (typeof((x).begin()) i = (x).begin(); i != (x).end(); i++)
#define SZ(x) ((int) (x).size())

template<class T>
void splitstr(const string &s, vector<T> &out)
{
    istringstream in(s);
    out.clear();
    copy(istream_iterator<T>(in), istream_iterator<T>(), back_inserter(out));
}

template<class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }

static void redirect(int argc, const char **argv)
{
    if (argc > 1)
    {
        int fd = open(argv[1], O_RDONLY);
        if (fd == -1) { perror(argv[1]); exit(1); }
        if (-1 == dup2(fd, 0)) { perror(argv[1]); exit(1); }
        if (-1 == close(fd)) { perror(argv[1]); exit(1); }
    }

    if (argc > 2)
    {
        int fd = open(argv[2], O_WRONLY | O_CREAT, 0666);
        if (fd == -1) { perror(argv[2]); exit(1); }
        if (-1 == dup2(fd, 1)) { perror(argv[2]); exit(1); }
        if (-1 == close(fd)) { perror(argv[2]); exit(1); }
    }
}

static ll dot(const pnt &a, const pnt &b) { return real(conj(a) * b); }
static ll cross(const pnt &a, const pnt &b) { return imag(conj(a) * b); }
static ll abs2(const pnt &a) { return dot(a, a); }

struct compare_angle
{
    bool operator()(const pair<pnt, int> &A, const pair<pnt, int> &B) const
    {
        pnt a = A.first;
        pnt b = B.first;
        if (a.imag() >= 0 && b.imag() < 0) return true;
        if (b.imag() >= 0 && a.imag() < 0) return false;

        ll c = cross(a, b);
        if (c != 0) return c > 0;

        if (B.second == -1) return false;
        else if (A.second == -1) return true;
        else return abs2(a) > abs2(b);
    }
};

static void up(int &r, int N)
{
    r++;
    if (r == N - 1) r = 0;
}

static int mod(int a, int b)
{
    a %= b;
    if (a < 0) a += b;
    return a;
}

typedef pair<pnt, int> ppi;

static int N;
static int next[1001][1001];
static vector<pnt> pnts;
static vector<vector<ppi> > ordered;

static int step(const pnt &src, int trg, long double r)
{
    const pnt &delta = pnts[trg] - src;

    vector<ppi>::const_iterator pos = lower_bound(RA(ordered[trg]), ppi(delta, -1), compare_angle());
    if (pos == ordered[trg].end())
        pos = ordered[trg].begin();

    int best = 0;
    cerr << "X\n";
    while (abs(pos->first) > r - 1e-9)
    {
        cerr << "Y\n";
        int p = next[trg][pos->second];
        if (p == -1) return best;
        assert(p >= 0 && p < SZ(ordered[trg]));
        pos = ordered[trg].begin() + p;
        cerr << "Z\n";
    }
    cerr << "W\n";

    while (true)
    {
        best = max(best, 1 + step(pnts[trg], pos->second, r - abs(pos->first)));
        r = abs(pos->first);

        int p = next[trg][pos->second];
        if (p == -1) return best;
        pos = ordered[trg].begin() + p;
    }
}

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;
    for (int cas = 0; cas < cases; cas++)
    {
        ll R;
        cin >> N >> R;

        pnts.clear();
        ordered.clear();
        pnts.resize(N);
        ordered.resize(N);
        for (int i = 0; i < N; i++)
        {
            cin >> pnts[i].real() >> pnts[i].imag();
            pnts[i] = -pnts[i];
        }
        if (N == 1)
        {
            printf("Case #%d: 0\n", cas + 1);
            continue;
        }
        for (int i = 0; i < N; i++)
        {
            vector<pair<pnt, int> > order;
            for (int j = 0; j < N; j++)
                if (i != j)
                    order.push_back(make_pair(pnts[j] - pnts[i], j));
            sort(RA(order), compare_angle());
            ordered[i] = order;

            vector<pair<pnt, int> > st;
            for (int pass = 0; pass < 3; pass++)
            {
                for (int j = N - 2; j >= 0; j--)
                {
                    while (!st.empty() && abs2(st.back().first) >= abs2(order[j].first))
                        st.pop_back();
                    if (st.empty())
                    {
                        next[i][order[j].second] = -1;
                    }
                    else
                    {
                        next[i][order[j].second] = st.back().second;
                    }
                    st.push_back(ppi(order[j].first, j));
                }
            }
        }
        printf("Case #%d: %d\n", cas + 1, step(pnt(1.0, 0.0), 0, R));
    }
    return 0;
}
