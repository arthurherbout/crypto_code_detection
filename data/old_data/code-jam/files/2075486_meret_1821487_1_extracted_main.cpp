#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <cassert>
#include <complex>

using namespace std;

#define ST first
#define ND second
#define MP make_pair
#define PB push_back

typedef long long LL;
typedef long double LD;

typedef vector<int> VI;
typedef pair<int,int> PII;
#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(i,a,b) for(VAR(i,a);i<=(b);++i)
#define FORD(i,a,b) for(VAR(i,a);i>=(b);--i)
#define FORE(a,b) for(VAR(a,(b).begin());a!=(b).end();++a)
#define VAR(a,b) __typeof(b) a=(b)
#define SIZE(a) ((int)((a).size()))
#define ALL(x) (x).begin(),(x).end()
#define CLR(x,a) memset(x,a,sizeof(x))
#define ZERO(x) memset(x,0,sizeof(x))

#define fabsl __builtin_fabsl
#define atan2l __builtin_atan2l
#define cosl __builtin_cosl
#define sinl __builtin_sinl
#define sqrtl __builtin_sqrtl

const LL INF = (LL) 1e17l;

struct Tile {
    LL x, y;
    char color;
};

struct Implication {
    LL vl, vr;
    LL min_w, max_w;
};

struct Event {
    Implication *imp;
    bool add;
    LL time;
};

bool operator < (const Event& a, const Event& b) {
    return a.time < b.time;
}

struct Point {
    LL x, y;
};

bool operator < (const Point& a, const Point& b) {
    if (abs(a.x) + abs(a.y) != abs(b.x) + abs(b.y)) {
        return abs(a.x) + abs(a.y) < abs(b.x) + abs(b.y);
    } else if (a.x != b.x) {
        return a.x > b.x;
    } else {
        return a.y > b.y;
    }
}

void alg() {
    int n;
    cin >> n;

    vector<Tile> tiles(n);
    Point *result = NULL;

    for (int i = 0; i < n; ++i) {
        cin >> tiles[i].x >> tiles[i].y >> tiles[i].color;
    }
    for (int rx = 0; rx < 2; ++rx) {
        for (int ry = 0; ry < 2; ++ry) {
            vector<Implication> implications;
            FORE (it, tiles) {
                if ((abs(it->x) % 2 != rx) == (abs(it->y) % 2 != ry)) {
                    if ((it->color == '.') != (abs(it->x) % 2 == rx)) {
                        goto bad;
                    }
                } else if (abs(it->x) % 2 == rx && abs(it->y) % 2 != ry) {
                    if (it->color == '#') {
                        {
                            Implication imp;
                            imp.vl = it->x + it->y + 1;
                            imp.vr = INF;
                            imp.min_w = -INF;
                            imp.max_w = it->x - it->y - 1;
                            implications.push_back(imp);
                        }
                        {
                            Implication imp;
                            imp.vl = -INF;
                            imp.vr = it->x + it->y;
                            imp.min_w = it->x - it->y;
                            imp.max_w = INF;
                            implications.push_back(imp);
                        }
                    } else {
                        {
                            Implication imp;
                            imp.vl = it->x + it->y + 1;
                            imp.vr = INF;
                            imp.min_w = it->x - it->y;
                            imp.max_w = INF;
                            implications.push_back(imp);
                        }
                        {
                            Implication imp;
                            imp.vl = -INF;
                            imp.vr = it->x + it->y;
                            imp.min_w = -INF;
                            imp.max_w = it->x - it->y - 1;
                            implications.push_back(imp);
                        }
                    }
                } else {
                    if (it->color == '#') {
                        {
                            Implication imp;
                            imp.vl = it->x + it->y;
                            imp.vr = INF;
                            imp.min_w = it->x - it->y;
                            imp.max_w = INF;
                            implications.push_back(imp);
                        }
                        {
                            Implication imp;
                            imp.vl = -INF;
                            imp.vr = it->x + it->y;
                            imp.min_w = -INF;
                            imp.max_w = it->x - it->y;
                            implications.push_back(imp);
                        }
                    } else {
                        {
                            Implication imp;
                            imp.vl = it->x + it->y;
                            imp.vr = INF;
                            imp.min_w = -INF;
                            imp.max_w = it->x - it->y;
                            implications.push_back(imp);
                        }
                        {
                            Implication imp;
                            imp.vl = -INF;
                            imp.vr = it->x + it->y;
                            imp.min_w = it->x - it->y;
                            imp.max_w = INF;
                            implications.push_back(imp);
                        }
                    }
                }
            }
            {
                multiset<LL, greater<LL> > lower_w;
                multiset<LL> upper_w;
                lower_w.insert(-INF);
                upper_w.insert(INF);
                vector<Event> events;
                FORE (it, implications) {
                    {
                        Event e;
                        e.time = it->vl;
                        e.imp = &*it;
                        e.add = true;
                        events.PB(e);
                    }
                    {
                        Event e;
                        e.time = it->vr + 1;
                        e.imp = &*it;
                        e.add = false;
                        events.PB(e);
                    }
                }
                {
                    Event e;
                    e.time = -INF;
                    e.imp = NULL;
                    events.PB(e);
                }
                {
                    Event e;
                    e.time = INF;
                    e.imp = NULL;
                    events.PB(e);
                }

                sort(ALL(events));
                FORE (it, events) {
                    if (it->imp != NULL) {
                        if (it->add) {
                            lower_w.insert(it->imp->min_w);
                            upper_w.insert(it->imp->max_w);
                        } else {
                            lower_w.erase(lower_w.find(it->imp->min_w));
                            upper_w.erase(upper_w.find(it->imp->max_w));
                    }
                    }
                    if (it + 1 == events.end() || (it + 1)->time != it->time) {
                        LL vl = it->time;
                        LL vr = (it + 1 == events.end()) ? INF : (it + 1)->time - 1;
                        LL min_w = *lower_w.begin();
                        LL max_w = *upper_w.begin();

                        if (abs(vl) % 2 != abs(rx + ry) % 2) {
                            ++vl;
                        }
                        if (abs(vr) % 2 != abs(rx + ry) % 2) {
                            --vr;
                        }
                        if (abs(min_w) % 2 != abs(rx - ry) % 2) {
                            ++min_w;
                        }
                        if (abs(max_w) % 2 != abs(rx - ry) % 2) {
                            --max_w;
                        }
                        if (vl <= vr && min_w <= max_w) {

                            vector<LL> options;
                            options.PB(vl);
                            options.PB(vr);
                            options.PB(-vl);
                            options.PB(-vr);
                            options.PB(min_w);
                            options.PB(max_w);
                            options.PB(-min_w);
                            options.PB(-max_w);
                            options.PB(abs(rx + ry) % 2);
                            options.PB(-abs(rx + ry) % 2);
                            {
                                vector<LL> other;
                                FORE (it, options)  {
                                    other.PB(*it - 1);
                                    other.PB(*it);
                                    other.PB(*it + 1);
                                }
                                options = other;
                            }
                            sort(ALL(options));
                            options.erase(unique(ALL(options)), options.end());
                            FORE (v, options) {
                                FORE (w, options) {
                                    if (vl <= *v && *v <= vr && min_w <= *w && *w <= max_w) {
                                        LL x = (*v + *w) / 2;
                                        LL y = (*v - *w) / 2;
                                        if (abs(x) % 2 != rx || abs(y) % 2 != ry) {
                                            continue;
                                        }
                                        Point *p = new Point();
                                        p->x = x;
                                        p->y = y;

                                        if (p != NULL && (result == NULL || *p < *result)) {
                                            result = p;
                                        } else {
                                            delete p;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            bad:{}
        }
    }
    if (result == NULL) {
        cout << "Too damaged" << endl;
    } else {
        cout << result->x << " " << result->y << endl;
    }
    delete result;
}

int main() {
    int d;
    cin >> d;
    for (int test_case = 1; test_case <= d; ++test_case) {
        cout << "Case #" << test_case << ": ";
        alg();
    }
}
