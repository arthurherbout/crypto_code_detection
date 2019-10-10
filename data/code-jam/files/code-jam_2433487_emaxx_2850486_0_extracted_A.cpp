#define _CRT_SECURE_NO_DEPRECATE
#define _SECURE_SCL 0
#pragma comment (linker, "/STACK:200000000")
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

typedef long long int64;
//typedef double old_double;
//#define double long double
const int INF = (int) 1E9;
const int64 INF64 = (int64) 1E18;
const double EPS = 1E-9;
const double PI = acos((double)0) * 2;

#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define ford(i,n)  for (int i=int(n)-1; i>=0; --i)
#define fore(i,l,n)  for (int i=int(l); i<int(n); ++i)
#define all(a)  a.begin(), a.end()
#define fs  first
#define sc  second
#define pb  push_back
#define mp  make_pair


int n;
int64 b, x[100];


void read() {
    cin >> b >> n;
    forn(i, n)
        cin >> x[i];
}


void solve() {
    vector<int64> cand;

    forn(i, n) {
        cand.push_back(x[i]);
        cand.push_back(x[i] - 1);
        cand.push_back(x[i] + 1);
    }

    cand.push_back(0);

    sort(x, x + n);
    int64 sum = 0;
    forn(i, n + 1) {
        for (int c = -4; c <= 4; ++c)
            cand.push_back((b + sum) / 37 + c);
        if (i < n)
            sum += x[i];
    }

    sort(all(cand));
    cand.erase(unique(all(cand)), cand.end());
    
    double ans = 0;
    forn(i, cand.size()) {
        int64 mi = cand[i];
        if (mi < 0)
            continue;

        fore(cnt, 1, 38) {

            multiset<int64> bets;
            int64 need = 0;
            forn(ii, 37 - n) {
                bets.insert(mi);
                need += mi;
            }
            
            forn(j, n)
                if (x[j] <= mi) {
                    bets.insert(mi - x[j]);
                    need += mi - x[j];
                }
            while ((int)bets.size() > cnt) {
                bets.erase(bets.begin());
                ++need;
            }

            if (need > b)
                continue;

            int64 sum_bets = 0;
            for (multiset<int64>::iterator it = bets.begin(); it != bets.end(); ++it)
                sum_bets += *it;

            double win_exp = 36.0 * sum_bets / cnt;
            double cans = win_exp - need;
            ans = max(ans, cans);
        }
    }
    printf("%.20lf\n", ans);
}


int main(int argc, char* argv[]) {
#ifdef SU2_PROJ
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
#endif

    int desired_test;
    if (argc < 2) {
        desired_test = -1;
        cerr << "Desired all tests." << endl;
    }
    else {
        desired_test = atoi(argv[1]);
        cerr << "Desired only test #" << desired_test << "." << endl;
    }

	int tests;
	cin >> tests;
	for (int test = 1; test <= tests; ++test) {
		read();
		if (!cin)
            throw runtime_error("Failed to read test!");

        if (desired_test != -1 && test != desired_test)
            continue;

        cerr << "Executing test #" << test << "..." << endl;
        cout << "Case #" << test << ": ";
		solve();
	}
    cerr << "Finish." << endl;
}
