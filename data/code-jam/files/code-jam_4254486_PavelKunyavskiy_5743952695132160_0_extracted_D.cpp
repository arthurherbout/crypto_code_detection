//#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <cstring>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <iterator>
#include <functional>
#include <bitset>
#include <stdarg.h>

#define mp make_pair
#define pb push_back

#ifdef LOCAL
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define eprintf(...)
#endif

#define TIMESTAMP(x) eprintf("["#x"] Time : %.3lf s.\n", clock()*1.0/CLOCKS_PER_SEC)
#define TIMESTAMPf(x, ...) eprintf("[" x "] Time : %.3lf s.\n", __VA_ARGS__, clock()*1.0/CLOCKS_PER_SEC)

#if (_WIN32 || __WIN32__)
    #define LLD "%I64d"
#else
#define LLD "%lld"
#endif

using namespace std;

#define TASKNAME "D"
#define TASKMOD "small"

typedef long long ll;
typedef long double ld;


typedef vector<pair<long long int, long long int>> vpll;
int _solved, _testNum;

class AbstractSolver {
    char *_ob;
    int _ob_pos;
    const int _buffer_size = 16 * (1 << 20);
public:
    int _testId;

    AbstractSolver(int _testId) : _testId(_testId) {
        _ob = (char *) malloc(_buffer_size + 10);
        _ob_pos = 0;
        printf("Case #%d: ", _testId);
    }

    __attribute__((format(printf, 2, 3)))
    void printf(const char *format, ...) {
        va_list ap;
        va_start (ap, format);
        _ob_pos += vsnprintf(_ob + _ob_pos, _buffer_size - _ob_pos, format, ap);
        va_end (ap);
        assert(_ob_pos < _buffer_size - 10);
    }

    virtual void write() {
        _ob[_ob_pos] = 0;
        ::printf("%s", _ob);
    }

    virtual void solve() = 0;;
    virtual void read() = 0;

    void run() {
        solve();
#pragma omp critical 
        {
            _solved++;
            if ((_solved - 1) * 100 / _testNum != _solved * 100 / _testNum || _solved <= 20 || _testNum - _solved <= 20)
                TIMESTAMPf("%d%% of tests solved (%d/%d, %d more)", _solved * 100 / _testNum, _solved, _testNum,
                           _testNum - _solved);
        }
    }
};


void PreCalc() {
}

class Solver : public AbstractSolver {
public:
    Solver(int _testId) : AbstractSolver(_testId) { }

    vpll vv;

    void read() {
        int t;
        scanf("%d", &t);
        vv.resize(t);
        for (auto &x : vv)
            scanf("%lld", &x.first);
        for (auto &x : vv)
            scanf("%lld", &x.second);
    }

    vpll remove(vpll v, ll val) {
        assert(val);
        vpll res;
        int start = val > 0 ? (int)v.size() - 1 : 0;
        int diff = val > 0 ? -1 : 1;
        int end = val > 0 ? -1 : (int)v.size();
        for (int i = start; i != end; i += diff) {
            if (v[i].second != 0) {
                res.push_back(mp(v[i].first - val, v[i].second));
                int id = lower_bound(v.begin(), v.end(), mp(v[i].first - val, -1LL)) - v.begin();
                if (id == (int) v.size() || v[id].first != v[i].first - val) return vpll();
                if (v[id].second < v[i].second) return vpll();
                v[id].second -= v[i].second;
            }
        }
        sort(res.begin(), res.end());
        return res;
    }

    vector<long long> total_res;


    bool go(vpll v, vector<long long> res) {
        /*for (auto x : v) {
            for (int i = 0; i < x.second; i++)
                eprintf("%lld ", x.first);
        }
        eprintf("\n");*/
        if (v.size() == 1 && v[0] == mp(0LL, 1LL)) {
            sort(res.begin(), res.end());
            if (total_res.empty()) total_res = res;
            total_res = min(res, total_res);
            return true;
        }

        if (v[0].second > 1) {
            int ctz = __builtin_ctz(v[0].second);
            //eprintf("%d zeros\n", ctz);
            assert(v[0].second == (1LL << ctz));
            for (auto &x: v) {
                assert(x.second % (1LL << ctz) == 0);
                x.second /= 1LL << ctz;
            }
            for (int i = 0; i < ctz; i++)
                res.push_back(0);
            return go(v, res);
        }
        if (v.size() == 1) return false;
        long long val = v[1].first - v[0].first;
        //eprintf("have %lld or %lld [%lld, %lld]\n", val, -val, v[1].first, v[0].first);
        for (int i = -1; i <= 1; i += 2) {
            vpll x = remove(v, i * val);
            if (!x.empty()) {
                //eprintf("trying %lld\n", i * val);
                res.push_back(i*val);
                go(x, res);//) return true;
                res.pop_back();
            }
        }
        return false;
    }

    void solve() {
        go(vv, vector<long long>());
        /*if (total_res.empty() || _testNum == 85){
            fprintf(stderr, "Fail\n");
            for (auto x : vv){
                eprintf("%lld ", x.first);
            }
            eprintf("\n");
            for (auto x : vv){
                eprintf("%lld ", x.second);
            }
            eprintf("\n");
            assert(0);
        }*/
        for (auto x : total_res){
            printf("%lld ", x);
        }
        printf("\n");
    }

};

int main() {
    freopen(TASKNAME "-" TASKMOD".in", "r", stdin);
    freopen(TASKNAME "-" TASKMOD".out", "w", stdout);

    PreCalc();
    TIMESTAMP(PreCalc);

    char buf[1000];
    fgets(buf, sizeof buf, stdin);
    sscanf(buf, "%d", &_testNum);

    vector<Solver *> solvers(_testNum);

    for (int i = 0; i < _testNum; i++) {
        solvers[i] = new Solver(i + 1);
        solvers[i]->read();
    }

#pragma omp parallel for
    for (int i = 0; i < _testNum; i++) {
        solvers[i]->run();
    }

    for (int i = 0; i < _testNum; i++) {
        solvers[i]->write();
    }

    TIMESTAMP(end);
    return 0;
}
