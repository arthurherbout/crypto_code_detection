#if 1
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>
using namespace std;

typedef long long LL;
typedef long double LD;
typedef pair<int , int> pii;
typedef vector <int> veci;
typedef vector <veci> graph;
const LD eps = 1e-9;
const LD pi = acos(-1.0);
const int inf = 1000 * 1000 * 1000;
const LL inf64 = LL(inf) * inf;

#define mp make_pair
#define pb push_back
#define X first
#define Y second
#define iss istringstream
#define oss ostringstream
#define dbg(x) {cerr << #x << " = " << x << endl;}
#define dbgv(x) {cerr << #x << " ={"; for (int _i = 0; _i < x.size(); _i++) {if (_i) cerr << ", "; cerr << x[_i];} cerr << "}" << endl;}
#define NAME "problem"

template<class T> string to_str(const T &a) { oss os; os << a; return os.str(); }
template<> string to_str<LD>(const LD& a) { oss os; os.precision(10); os.setf(ios::fixed); os << a; return os.str(); }
template<class T> T from_str(const string &s) { iss is; T val; is >> val; return val; }
string T(int test) { ostringstream os; os << "Case #" << test << ":"; return os.str(); }
int _test_start = -1, _test_end = -1;
bool need_to_run(int test) { return _test_start == -1 || _test_start <= test && test <= _test_end; }

struct event_t
{
    int type; // 0 - enter, 1 - leave
    int who; 
    event_t() {}
    event_t(int type, int who) : type(type), who(who) {}
};
vector< event_t > events;
vector<int> ord;
vector<int> ids;
int n;
int best = -1;
int here[20];
void go(int pos)
{
    if (pos == n)
    {
        int cnt = 0;
        int masks = 0;
        memset(here, -1, sizeof here);
        for (int i = 0; i < n; ++i)
        {
            if (ord[i] == 0)
            {
                if (events[i].type == 0)
                    ++masks;
                else
                    --masks;
                if (masks < 0)
                    masks = 0;
                continue;
            }
            if (events[i].type == 0)
            {
                if (here[ord[i]] == 1)
                    return;
                here[ord[i]] = 1;
                ++cnt;
            } else
            {
                if (here[ord[i]] == 0)
                    return;
                if (here[ord[i]] != -1)
                    --cnt;
                here[ord[i]] = 0;
                
            }
        }
        cnt += masks;
        if (best == -1 || best > cnt)
            best = cnt;
        return;
    }

    if (events[pos].who != 0)
    {
        ord[pos] = events[pos].who;
        go(pos + 1);
    } else
    {
        for (int i = 0; i < ids.size(); ++i)
        {
            ord[pos] = i;
            go(pos + 1);
        }
    }

}
void solve(int test)
{
    // read
    cin >> n;
    events.clear();
    ids.clear();
    ids.push_back(0);
    for (int i = 0; i < n; ++i)
    {
        string s;
        int id;
        cin >> s >> id;
        ids.push_back(id);
        int type = s[0] == 'E' ? 0 : 1;
        events.push_back(event_t(type, id));
    }
    sort(ids.begin(), ids.end());
    ids.erase(unique(ids.begin(), ids.end()), ids.end());
    for (int i = 0; i < n; ++i)
        events[i].who = lower_bound(ids.begin(), ids.end(), events[i].who) - ids.begin();

    if (!need_to_run(test)) return;
    // solve
    ord.assign(n, 0);
    best = -1;
    go(0);

    // write
    if (best == -1)
        cout << T(test) << " CRIME TIME" << endl;
    else
        cout << T(test) << " " << best << endl;

}

int main(int argc, char *argv[])
{
    freopen("input.txt", "r", stdin); freopen("output.txt", "w", stdout);
    //freopen(NAME ".in","r",stdin); freopen(NAME ".out","w",stdout);
    if (argc == 2) { _test_start = _test_end = from_str<int>(argv[1]); }
    if (argc == 3) { _test_start = from_str<int>(argv[1]); _test_end = from_str<int>(argv[2]); }

    clock_t tstart = clock();

    int tests;
    scanf("%d", &tests);
    for(int test = 1; test <= tests; ++test)
    {
        clock_t tprev = clock();
        solve(test);
        if (need_to_run(test))
            dbg("elapsed for #" << test << ": " << (clock() - tprev) / LD(CLOCKS_PER_SEC));
    }

    dbg("elapsed: " << (clock() - tstart) / LD(CLOCKS_PER_SEC));
    return 0;
}
/*************************
*************************/
#endif

