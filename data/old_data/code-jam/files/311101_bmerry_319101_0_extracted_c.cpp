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
#include <ext/hash_map>

using namespace std;
using namespace __gnu_cxx;

typedef vector<int> vi;
typedef vector<string> vs;
typedef long long ll;
typedef complex<double> pnt;
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

#define MOD 10007

struct hash_ll
{
    size_t operator()(ll x) const
    {
        return x % 1000000007;
    }
};

static int R, C;
static char grid[11][12];
static hash_map<ll, int, hash_ll> cache;

static ll make_key(const char *base, int r, int split)
{
    ll key = 0;
    for (int i = 0; i < C; i++)
        if (base[i] != '.')
            key = (key << 5) | (base[i] - 'a' + 1);
    key = (key << 4) | r;
    key = (key << 4) | split;
    return key;
}

static int recurse(char *base, int r, int split)
{
    if (split == C)
    {
        r++;
        split = 0;
    }
    if (r == R)
        return 1;

    ll key = make_key(base, r, split);
    if (cache.count(key))
        return cache[key];

    int up = base[split];
    char low = up;
    if (split > 0) low = max(low, base[split - 1]);
    if (low < 'a') low = 'a';

    if (grid[r][split] != '.')
    {
        if (grid[r][split] < low)
            return 0;
        else
        {
            base[split] = grid[r][split];
            int ans = recurse(base, r, split + 1);
            base[split] = up;
            return ans;
        }
    }

    int ans = 0;
    for (char c = low; c <= 'z'; c++)
    {
        base[split] = c;
        ans += recurse(base, r, split + 1);
    }
    base[split] = up;
    ans %= MOD;
    return cache[key] = ans;
}

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;
    for (int cas = 0; cas < cases; cas++)
    {
        cin >> R >> C;
        for (int i = 0; i < R; i++)
            cin >> grid[i];
        cache.clear();

        char base[12] = {};
        fill(base, base + C, '.');
        int ans = recurse(base, 0, 0);
        printf("Case #%d: %d\n", cas + 1, ans);
    }
    return 0;
}
