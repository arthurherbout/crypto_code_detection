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

#define TASKNAME "B"
#define TASKMOD "small"

typedef long long ll;
typedef long double ld;


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

    int n, k;
    vector<int> sum;
    void read() {
        scanf("%d%d",&n, &k);
        sum.resize(n - k + 1);
        for (int& x : sum)
            scanf("%d",&x);
    }

    void solve() {
        vector<int> v;
        int sum0 = 0;
        int sum1 = 0;
        for (int i = 0; i < k; i++){
            int minv = 0;
            int maxv = 0;
            int cur = 0;
            for (int j = i; j + 1 < (int)sum.size(); j += k){
                cur += sum[j+1] - sum[j];
                minv = min(minv, cur);
                maxv = max(maxv, cur);
            }
            sum0 += minv;
            sum1 += maxv;
            v.push_back(maxv - minv);
        }
        sort(v.begin(), v.end());
        int cnt0 = ((sum[0] - sum0) % k + k) % k;
        for (int i = 0; i < k; i++) {
            cnt0 -= v.back() - v[i];
        }
        int cnt1 = ((sum[0] - sum1) % k + k) % k;
        for (int i = 0; i < k; i++) {
            cnt1 -= v.back() - v[i];
        }
        if (cnt0 <= 0 || cnt1 <= 0)
            printf("%d\n", v.back());
        else
            printf("%d\n", v.back() + 1);
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
