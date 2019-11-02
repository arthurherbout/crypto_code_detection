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


const int MAXN = 20;


string s;

void read() {
    cin >> s;
}


int n;
double pr[1 << MAXN], d[1 << MAXN];


void solve() {
    memset(d, 0, sizeof d);
    memset(pr, 0, sizeof pr);
    n = (int)s.length();

    int start = 0;
    forn(i, n)
        if (s[i] == 'X')
            start |= 1 << i;
    pr[start] = 1;

    forn(msk, (1 << n) - 1) {
        double mypr = pr[msk];
        double my = d[msk];
        if (abs(mypr) < 1E-12)
            continue;

        int slen = 0;
        forn(last, n)
            if (!(msk & (1 << last))) {
                int nmsk = msk + (1 << last);

                double cost = 0;
                int len = 0;
                for (int i = last; ; ) {
                    cost += n - len;
                    ++len;

                    if (--i < 0)
                        i += n;
                    if (!(msk & (1 << i)))
                        break;
                }
                slen += len;

                d[nmsk] += mypr * len / n * (my / mypr + cost / len);
                pr[nmsk] += mypr * len / n;
            }
        if (slen != n)
            throw;
    }
    if (abs(pr[(1 << n) - 1] - 1) > 1E-2)
        throw;
    printf("%.20lf\n", d[(1 << n) - 1]);
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
