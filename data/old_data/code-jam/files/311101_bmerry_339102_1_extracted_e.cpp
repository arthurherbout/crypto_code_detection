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

struct blob_entry
{
    int h[2];
    int edge;

    blob_entry() {}
    blob_entry(int h0, int h1, int e)
    {
        h[0] = h0;
        h[1] = h1;
        edge = e;
    }

    bool operator <(const blob_entry &b) const { return edge < b.edge; }
};

struct blob
{
    vector<blob_entry> entries;
    vector<int> sizes;

    bool operator < (const blob &b) const { return entries[0] < b.entries[0]; }
};

static vector<vector<int> > edges;
static vector<int> cols;
static vector<blob> blobs;

static bool do_color(int i, int col)
{
    if (cols[i] == 1 - col)
        return false;
    else if (cols[i] == col)
        return true;

    cols[i] = col;
    for (size_t j = 0; j < edges[i].size(); j++)
    {
        if (!do_color(edges[i][j], 1 - col)) return false;
    }
    return true;
}

static bool make_blobs(const vector<pii> &pairs)
{
    int N = pairs.size();
    cols.clear();
    edges.clear();
    blobs.clear();

    edges.resize(N);
    cols.resize(N, -1);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (pairs[i].first < pairs[j].first
                && pairs[j].first < pairs[i].second
                && pairs[i].second < pairs[j].second)
            {
                edges[i].push_back(j);
                edges[j].push_back(i);
            }

    for (int x = 0; x < N; x++)
        if (cols[x] == -1)
        {
            if (!do_color(x, 0)) return false;
            blob b;
            for (int i = 0; i < N; i++)
            {
                if (cols[i] == 0 || cols[i] == 1)
                {
                    b.entries.push_back(blob_entry(1 - cols[i], cols[i], pairs[i].first));
                    b.entries.push_back(blob_entry(cols[i] - 1, -cols[i], pairs[i].second));
                    cols[i] += 2;
                }
            }

            sort(RA(b.entries));
            int h[2] = {0, 0};
            for (size_t i = 1; i < b.entries.size(); i++)
            {
                h[0] = max(h[0], b.entries[i - 1].h[0]);
                h[1] = max(h[1], b.entries[i - 1].h[1]);
                b.entries[i].h[0] += b.entries[i - 1].h[0];
                b.entries[i].h[1] += b.entries[i - 1].h[1];
            }

            b.sizes.resize(N + 1);
            sort(h, h + 2);
            for (int i = 0; i < h[0]; i++)
                b.sizes[i] = INT_MAX / 2;
            for (int i = h[0]; i < h[1]; i++)
                b.sizes[i] = h[1];
            for (int i = h[1]; i <= N; i++)
                b.sizes[i] = h[0];
            blobs.push_back(b);
        }
    sort(RA(blobs));
    return true;
}

static void unify_sizes(vector<int> &sizes1, const vector<int> &sizes2, int ofs1, int ofs2)
{
    for (int i = 0; i < SZ(sizes1); i++)
    {
        if (i < ofs1)
            sizes1[i] = INT_MAX / 2;
        else
            sizes1[i] = max(sizes1[i], sizes2[i - ofs1] + ofs2);
    }
}

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;
    for (int cas = 0; cas < cases; cas++)
    {
        map<string, int> posmap;

        int N;
        cin >> N;
        vector<pii> pairs;
        for (int i = 0; i < 2 * N; i++)
        {
            string w;
            cin >> w;
            if (!posmap.count(w))
                posmap[w] = i;
            else
                pairs.push_back(pii(posmap[w], i));
        }

        int best = INT_MAX / 2;
        if (make_blobs(pairs))
        {
            for (int i = SZ(blobs) - 1; i >= 0; i--)
            {
                const vector<blob_entry> &e = blobs[i].entries;
                for (int j = i + 1; j < SZ(blobs); j++)
                {
                    vector<blob_entry>::const_iterator p = upper_bound(RA(e), blobs[j].entries[0]);
                    assert(p != e.begin());
                    if (p == e.end()) continue;
                    --p;
                    unify_sizes(blobs[i].sizes, blobs[j].sizes, p->h[0], p->h[1]);
                }
            }

            vector<int> sizes(N + 1, 0);
            for (int i = 0; i < SZ(blobs); i++)
                unify_sizes(sizes, blobs[i].sizes, 0, 0);

            for (int i = 0; i <= N; i++)
                best = min(best, sizes[i] + i);
        }
        if (best >= INT_MAX / 2)
            best = -1;
        printf("Case #%d: %d\n", cas + 1, best);
    }
    return 0;
}
