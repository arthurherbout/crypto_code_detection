#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const int N_MAX = 5005;
const int KEEP = 75000;

struct point
{
    int x, y;

    point(int _x = 0, int _y = 0)
    {
        x = _x;
        y = _y;
    }

    point operator + (const point &p) const
    {
        return point(x + p.x, y + p.y);
    }

    point operator - (const point &p) const
    {
        return point(x - p.x, y - p.y);
    }

    point rotate90(const point &p) const
    {
        point diff = (*this) - p;
        diff = point(diff.y, -diff.x);
        return p + diff;
    }

    bool operator < (const point &p) const
    {
        if (x != p.x)
            return x < p.x;

        return y < p.y;
    }

    double abs() const
    {
        return sqrt((double) x * x + (double) y * y);
    }

    double norm() const
    {
        return (double) x * x + (double) y * y;
    }
};

bool compare_dist(point a, point b)
{
    return a.norm() > b.norm();
}

int N, M;
point stars[N_MAX];

void initialize()
{

}

void solve_case(int test_case)
{
    scanf("%d %d", &N, &M);

    for (int i = 0; i < N; i++)
        scanf("%d %d", &stars[i].x, &stars[i].y);

    set<point> places;
    places.insert(point(0, 0));

    for (int m = 0; m < M; m++)
    {
        vector<point> to_insert;

        for (set<point>::iterator it = places.begin(); it != places.end(); it++)
        {
            point p = *it;

            for (int i = 0; i < N; i++)
                to_insert.push_back(p.rotate90(stars[i]));
        }

        for (int i = 0; i < (int) to_insert.size(); i++)
            places.insert(to_insert[i]);

        if ((int) places.size() > KEEP)
        {
            vector<point> v_places(places.begin(), places.end());
            nth_element(v_places.begin(), v_places.begin() + KEEP, v_places.end(), compare_dist);
            sort(v_places.begin(), v_places.begin() + KEEP);
            places = set<point>(v_places.begin(), v_places.begin() + KEEP);
        }
    }

    double best = 0;

    for (set<point>::iterator it = places.begin(); it != places.end(); it++)
        best = max(best, it->abs());

    printf("Case #%d: %.9lf\n", test_case, best);
}

int main()
{
    initialize();
    int T; scanf("%d", &T);

    for (int tc = 1; tc <= T; tc++)
    {
        solve_case(tc);
        fflush(stdout);
    }

    return 0;
}
