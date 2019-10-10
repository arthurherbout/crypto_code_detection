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
const long double PI = acos((-1.0l));
template<class T> void chmax(T & a, const T & b) { a = max(a, b); }
template<class T> void chmin(T & a, const T & b) { a = min(a, b); }

#define REP(i,n) for(int i=0; i<(n); i++)
#define FOR(i,a,b) for(int i=(a); i<(b); i++)
#define FORD(i,a,b) for(int i=(b)-1; i>=(a); i--)
#define FORV(i,v) for(int i=0; i<(v).size(); i++)
#define FORVD(i,v) for(int i=(v).size()-1; i>=0; i--)
#define FORE(it,c) for (__typeof((c).begin()) it=(c).begin(); it!=(c).end(); it++)
#define FORED(it,c) for (__typeof((c).rbegin()) it=(c).rbegin(); it!=(c).rend(); it++)
#define DEBUG(x) cout << '>' << #x << ':' << (x) << endl
#define SIZE(a) ((int)(a).size())
#define CLEAR(a) memset((a),0,sizeof(a))
#define ALL(a) (a).begin(),(a).end()


long double logfs[500000];

void precompute() {
    logfs[1] = 0;
    FOR(i, 2, 500000) {
        logfs[i] = logfs[i-1] + log((long double)(i));
        //DEBUG(logfs[i]);
    }
}

long double logf(int n) {
    return logfs[n];
}

long double log2n(int n) {
    long double two = 2.0;
    long double logtwo = log(two);
    return logtwo * n;
}

long double pr(int k, int n) {
    //DEBUG(k);
    //DEBUG(n);
    long double logres = logf(n) - logf(k) - log2n(n);
    return exp(logres);
}

long double calculate(int h, int di, int sp) {
    long double res = 0;
    int side = (sp-1)/2;
    if(h == side+1) return di == sp ? 1.0 : 0.0;
    FOR(i, 1, (di + 1)) {
        if(i >= h) res += pr(i, di);
    }
    return res;
}

int main() {
    precompute();
	//freopen("b.in", "r", stdin);
	int t;
	cin >> t;
    FOR(e, 1, t+1) {
        int n, x, y;
        cin >> n >> x >> y;
        x = abs(x);
        int myround = (x+y) / 2;
        int rounds = (n-1) / 4;
        int inmyround = n - (4*rounds + 1);
        int myroundsize = (4*(rounds+1) + 1);
        //DEBUG(rounds);
        //DEBUG(myround);
        long double answer = 0;
        if(myround <= rounds) {
            answer = 1;
        }
        else if(myround > rounds+1) {
            answer = 0;
        }
        else {
            //DEBUG(y+1);
            //DEBUG(inmyround);
            //DEBUG(myroundsize);
            answer = calculate(y+1, inmyround, myroundsize);
        }
        cout << "Case #" << e << ": " << answer << endl;
	}
	return 0;
}

