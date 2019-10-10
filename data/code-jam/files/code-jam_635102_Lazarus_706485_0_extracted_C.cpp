#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <iostream>
#include <queue>
#include <list>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct rect_t
{
    int X1, Y1, X2, Y2;
};

struct point_t
{
    int x, y;
    point_t(int _x, int _y) : x(_x), y(_y) {}
};

bool
operator< (const point_t& lhs, const point_t& rhs)
{
    return (lhs.x < rhs.x) ||
        (lhs.x == rhs.x && lhs.y < rhs.y);
}

int main(int argc, char *argv[])
{
    int ncases;
    cin >> ncases;
    for(int i = 1; i <= ncases; ++i)
    {
        int R;
        cin >> R;

        typedef set<point_t> grid_t;
        grid_t b;
        vector<rect_t> rects;
        for(int j = 0; j < R; ++j)
        {
            rect_t rect;
            cin >> rect.X1 >> rect.Y1 >> rect.X2 >> rect.Y2;
            rects.push_back(rect);
            for(int x = rect.X1; x <= rect.X2; ++x)
                for(int y = rect.Y1; y <= rect.Y2; ++y)
                    b.insert(point_t(x, y));
        }

        int steps = 0;
        while(!b.empty())
        {
//            cout << "Steps " << steps << "; size = " << b.size() << endl;
            grid_t b_new;
            for(grid_t::const_iterator it = b.begin(); it != b.end(); ++it)
            {
                point_t p = *it;
//                cout << "Point " << p.x << ", " << p.y << endl;
                bool bact_west = b.find(point_t(p.x - 1, p.y)) != b.end();
                bool bact_north = b.find(point_t(p.x, p.y - 1)) != b.end();
                if(bact_west || bact_north)
                    b_new.insert(p);
                point_t p_east = point_t(p.x + 1, p.y);
                point_t p_south = point_t(p.x, p.y + 1);
                if(b.find(p_east) == b.end() && b.find(point_t(p.x + 1, p.y - 1)) != b.end())
                    b_new.insert(p_east);
                if(b.find(p_south) == b.end() && b.find(point_t(p.x - 1, p.y + 1)) != b.end()) {
                    b_new.insert(p_south);
                }
            }
            b = b_new;
            ++steps;
        }

        cout << "Case #" << i << ": ";
        cout << steps;
        cout << endl;
    }
}
