#pragma comment (linker, "/STACK:200000000")
#define _SECURE_SCL 0
#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <stack>
#include <sstream>
#include <vector>

using namespace std;

typedef long long int64;
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


const int MAXN = 4010;
const int DICTN = 521196;


char dict[DICTN][30];
int dictlen[DICTN];

char s[MAXN];
int n;


void read() {
    scanf(" %s", s);
    n = (int)strlen(s);
}


int d[MAXN][6];


void solve() {
    forn(i, n + 1)
        forn(j, 6)
            d[i][j] = INF;

    d[0][5] = 0;
    forn(pos, n) {
        fore(dst, 1, 6) {
            int my = d[pos][dst];
            if (my == INF)
                continue;

            forn(i, DICTN) {
                int patlen = dictlen[i];
                if (pos + patlen > n)
                    continue;

                const char* expected = s + pos;
                const char* pattern = dict[i];

                if (patlen >= 2 && expected[0] != pattern[0] && expected[1] != pattern[1])
                    continue;
                if (patlen >= 3 && expected[1] != pattern[1] && expected[2] != pattern[2])
                    continue;
                if (patlen >= 2 && expected[patlen - 1] != pattern[patlen - 1] && expected[patlen - 2] != pattern[patlen - 2])
                    continue;

                int worstans = -INF;
                fore(j, 1, 6)
                    worstans = max(worstans, d[pos + patlen][j]);
                if (my >= worstans)
                    continue;

                int cdst = dst;
                int cans = my;
                forn(j, patlen) {
                    if (*expected == *pattern)
                        ++cdst;
                    else {
                        if (cdst < 5) {
                            cans = INF;
                            break;
                        }
                        ++cans;
                        cdst = 1;

                        if (cans >= worstans) {
                            cans = INF;
                            break;
                        }
                    }

                    ++expected;
                    ++pattern;
                }

                if (cans < INF) {
                    cdst = min(cdst, 5);
                    d[pos + patlen][cdst] = min(d[pos + patlen][cdst], cans);
                }
            }
        }
        cerr << pos << ' ';
    }
    cerr << endl;

    int ans = INF;
    forn(dst, 6) {
        int my = d[n][dst];
        ans = min(ans, my);
    }
    cout << ans << endl;
}


int main() {
    freopen("dict.txt", "rt", stdin);
    forn(i, DICTN) {
        gets(dict[i]);
        dictlen[i] = (int)strlen(dict[i]);
    }
    cerr << "dict: " << clock() * 1.0 / CLOCKS_PER_SEC << endl;

    freopen("input.txt", "rt", stdin);
    freopen("output.txt", "wt", stdout);

    int ts;
    cin >> ts;
    for (int tt = 0; tt < ts; ++tt) {
        printf("Case #%d: ", tt + 1);

        read();
        if (!cin)
            throw;
        cerr << "start test #" << tt + 1 << endl;
        solve();
        cerr << "test #" << tt + 1 << ": " << clock() * 1.0 / CLOCKS_PER_SEC << endl;
    }
}
