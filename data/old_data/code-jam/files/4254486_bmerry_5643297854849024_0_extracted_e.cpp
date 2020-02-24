#include <bits/stdc++.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

/*** START OF TEMPLATE CODE ***/

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<double> pnt;
typedef pair<int, int> pii;

#define RA(x) begin(x), end(x)
#define FE(i, x) for (auto i = begin(x); i != end(x); ++i)
#define SZ(x) ((int) (x).size())

template<class T>
vector<T> splitstr(const string &s)
{
    istringstream in(s);
    vector<T> out;
    copy(istream_iterator<T>(in), istream_iterator<T>(), back_inserter(out));
    return out;
}

template<class T> T gcd(T a, T b) { return b ? gcd(b, a % b) : a; }

static void redirect(int argc, const char **argv)
{
    ios::sync_with_stdio(false);

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

    cin.exceptions(ios::failbit | ios::badbit);
}

/*** END OF TEMPLATE CODE ***/

struct cheater
{
};

static pair<ll, ll> solve(vector<ll> &c, int D)
{
    int N = SZ(c);
    if (N == 0)
        return {0, 0};

    ll ans = 0;
    ll nth = 0;
    for (int d = D; d >= 1; d /= 2)
    {
        for (int i = 0; i < d; i++)
        {
            if (i + d >= N)
                continue;
            ll gap = c[i + d] - c[i];
            if (gap % 2)
                throw cheater();
            gap /= 2;
            ans += llabs(gap);
            int j;
            for (j = i; j < N; j += d)
            {
                c[j] += gap;
                gap = -gap;
            }
            if (j == N)
                nth += gap;
        }
        for (int i = 0; i < N - d; i++)
            if (c[i] != c[i + d])
                throw cheater();

        if (2 * d > N)
        {
            for (int i = 0; i < d - 1; i++)
                if (c[i] != c[i + d])
                    throw cheater();
            ll mid = c[d - 1];
            c.resize(d - 1);
            auto sub = solve(c, d / 2);
            mid += sub.second;
            nth += sub.second;
            ans += sub.first + llabs(mid);
            nth += mid;
            return {ans, nth};
        }
    }

    if (count(RA(c), 0) != N)
        throw cheater();

    return {ans, nth};
}

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;
    for (int cas = 0; cas < cases; cas++)
    {
        ll ans = 0;
        try
        {
            int N, D;
            cin >> N >> D;
            vector<ll> d(N);
            for (int i = 0; i < N; i++)
                cin >> d[i];
            vector<ll> c(N - 1);
            for (int i = 0; i < N - 1; i++)
                c[i] = d[i + 1] - d[i];

            //if (count(RA(d), 0) == N)
            //    throw cheater();
            ans = solve(c, D).first;
        }
        catch (cheater &e)
        {
            cout << "Case #" << cas + 1 << ": CHEATERS!" << "\n";
            continue;
        }
        cout << "Case #" << cas + 1 << ": " << ans << "\n";
    }
    return 0;
}
