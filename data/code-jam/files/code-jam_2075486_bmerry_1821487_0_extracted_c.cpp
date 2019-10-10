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

struct cell
{
    ll x, y;
    ll ph;
};

struct compare_x
{
    bool operator()(const cell &a, const cell &b)
    {
        return a.x < b.x;
    }
};

struct compare_y
{
    bool operator()(const cell &a, const cell &b)
    {
        return a.y < b.y;
    }
};

#define MASK_X 1
#define MASK_Y 2

int mask(const cell &c, ll px, ll py)
{
    ll dx = (px - c.x) & 1;
    ll dy = (py - c.y) & 1;
    int ans = 0;
    if (dy == c.ph)
        ans |= MASK_Y;
    if (dx == c.ph)
        ans |= MASK_X;
    return ans;
}

struct box
{
    ll min_xpy;
    ll min_xmy;
    ll max_xpy;
    ll max_xmy;

    box() {}
    box(ll min_xpy, ll min_xmy, ll max_xpy, ll max_xmy)
        : min_xpy(min_xpy), min_xmy(min_xmy), max_xpy(max_xpy), max_xmy(max_xmy) {}
};

int main(int argc, const char **argv)
{
    redirect(argc, argv);

    int cases;
    cin >> cases;
    for (int cas = 0; cas < cases; cas++)
    {
        int N;
        cin >> N;
        vector<cell> xcells(N), ycells;
        for (int i = 0; i < N; i++)
        {
            char ch;
            cin >> xcells[i].x >> xcells[i].y >> ch;
            xcells[i].ph = ch == '#';
        }
        ycells = xcells;
        sort(RA(xcells), compare_x());
        sort(RA(ycells), compare_y());

        ll best_manhattan = LLONG_MAX;
        ll best_x = 0;
        ll best_y = 0;

        for (int px = 0; px < 2; px++)
            for (int py = 0; py < 2; py++)
            {
                vector<int> mx(N), my(N);
                vector<box> xboxes;
                vector<box> yboxes;
                for (int i = 0; i < N; i++)
                {
                    mx[i] = mask(xcells[i], px, py);
                    my[i] = mask(ycells[i], px, py);
                    if (mx[i] == 0)
                        goto badph;
                }

                for (int xsi = 0; xsi <= N; xsi++)
                {
                    ll max_xpy = LLONG_MIN / 4;
                    ll max_xmy = LLONG_MIN / 4;
                    ll min_xpy = LLONG_MAX / 4;
                    ll min_xmy = LLONG_MAX / 4;
                    for (int i = 0; i < xsi; i++)
                    {
                        if (mx[i] == MASK_X)
                        {
                            max_xpy = max(max_xpy, xcells[i].x + xcells[i].y);
                            max_xmy = max(max_xmy, xcells[i].x - xcells[i].y);
                        }
                    }
                    for (int i = xsi; i < N; i++)
                    {
                        if (mx[i] == MASK_X)
                        {
                            min_xpy = min(min_xpy, xcells[i].x + xcells[i].y);
                            min_xmy = min(min_xmy, xcells[i].x - xcells[i].y);
                        }
                    }
                    xboxes.push_back(box(max_xpy, max_xmy, min_xpy, min_xmy));
                }

                for (int ysi = 0; ysi <= N; ysi++)
                {
                    ll max_ypx = LLONG_MIN / 4;
                    ll max_ymx = LLONG_MIN / 4;
                    ll min_ypx = LLONG_MAX / 4;
                    ll min_ymx = LLONG_MAX / 4;
                    for (int i = 0; i < ysi; i++)
                    {
                        if (my[i] == MASK_Y)
                        {
                            max_ypx = max(max_ypx, ycells[i].y + ycells[i].x);
                            max_ymx = max(max_ymx, ycells[i].y - ycells[i].x);
                        }
                    }
                    for (int i = ysi; i < N; i++)
                    {
                        if (my[i] == MASK_Y)
                        {
                            min_ypx = min(min_ypx, ycells[i].y + ycells[i].x);
                            min_ymx = min(min_ymx, ycells[i].y - ycells[i].x);
                        }
                    }
                    yboxes.push_back(box(max_ypx, -min_ymx, min_ypx, -max_ymx));
                }

                for (int i = 0; i < SZ(xboxes); i++)
                    for (int j = 0; j < SZ(yboxes); j++)
                    {
                        ll min_xpy = max(xboxes[i].min_xpy, yboxes[i].min_xpy);
                        ll min_xmy = max(xboxes[i].min_xmy, yboxes[i].min_xmy);
                        ll max_xpy = min(xboxes[i].max_xpy, yboxes[i].max_xpy);
                        ll max_xmy = min(xboxes[i].max_xmy, yboxes[i].max_xmy);
                        if (min_xpy <= max_xpy && min_xmy <= max_xmy)
                        {
                            vector<ll> xs;
                            for (ll x = -2; x <= 2; x++)
                            {
                                xs.push_back(x);
                            }
                            for (int m = -2; m <= 2; m++)
                            {
                                xs.push_back(min_xpy + m);
                                xs.push_back(max_xpy + m);
                                xs.push_back(min_xmy + m);
                                xs.push_back(max_xmy + m);
                            }
                            for (int k = 0; k < 2; k++)
                                for (int l = 0; l < 2; l++)
                                {
                                    ll xpy = k ? min_xpy : max_xpy;
                                    ll xmy = l ? min_xmy : max_xmy;
                                    ll x = (xpy + xmy) / 2;
                                    for (ll m = -2; m <= 2; m++)
                                    {
                                        xs.push_back(x + m);
                                    }
                                }
                            for (int k = 0; k < SZ(xs); k++)
                            {
                                ll x = xs[k];
                                if ((x & 1) != px)
                                    continue;
                                ll yl = max(min_xpy - x, x - max_xmy);
                                ll yh = min(max_xpy - x, x - min_xmy);
                                if (yl <= yh)
                                {
                                    ll y0;
                                    if (yl <= 0 && yh >= 0)
                                        y0 = 0;
                                    else if (yl > 0)
                                        y0 = yl;
                                    else
                                        y0 = yh;
                                    for (ll y = y0 - 1; y <= y0 + 1; y++)
                                    {
                                        if ((y & 1) != py)
                                            continue;
                                        if (y < yl || y > yh)
                                            continue;
                                        ll manhattan = abs(x) + abs(y);
                                        if (manhattan < best_manhattan
                                            || (manhattan == best_manhattan &&
                                                (x > best_x || (x == best_x && y > best_y))))
                                        {
                                            best_manhattan = manhattan;
                                            best_x = x;
                                            best_y = y;
                                        }
                                    }
                                }
                            }
                        }
                    }
badph:
                (void) 0;
            }
        printf("Case #%d: ", cas + 1);
        if (best_manhattan == LLONG_MAX)
            printf("Too damaged\n");
        else
            printf("%lld %lld\n", best_x, best_y);
    }
    return 0;
}
