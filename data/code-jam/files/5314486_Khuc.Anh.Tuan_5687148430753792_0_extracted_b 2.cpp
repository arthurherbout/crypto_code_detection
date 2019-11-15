#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

#define Rep(i,n) for(int i=0;i<(n);++i)
#define Repd(i,n) for(int i=((int)(n))-1;i>=0;--i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define pb push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<int> VI;

#define debug cout << "Here " << __LINE__ << endl;
template <typename T> ostream& operator<<(ostream &os, vector<T> v) { Rep(i, v.size()) os << v[i] << " "; os << endl; return os; }
template <typename T1, typename T2> ostream& operator<<(ostream &os, pair<T1, T2> p) { os << "(" << p.fi << ", " << p.se << ")"; return os; }

int n, c, m;
int p[1010];
int inf = 1000000;
int F[1010];

int solve(int xn) {
    Rep(i, m) F[i] = inf;
    int cur = 0;
    Rep(i, m) {
        if (i > 0 && p[i] == p[i-1]) ++cur;
        else cur = 1;
        bool needPromotion = false;
        if (cur > xn) needPromotion = true;
        if (cur > xn * p[i]) F[i] = inf;
        else F[i] = min(F[i], (i > 0 ? F[i-1] : 0) + needPromotion);
    }
    return F[m-1];
}

int main() {
    int nt;
    cin >> nt;
    Rep(t, nt) {
        cin >> n >> c >> m;
        VI count(c, 0);
        Rep(i, m) {
            int z;
            cin >> p[i] >> z;
            count[z-1]++;
        }
        sort(p, p + m);
        int mink = *max_element(count.begin(), count.end());
        int left = mink, right = 1010;
        while (left < right) {
            int mid = (left + right) / 2;
            if (solve(mid) < inf) right = mid;
            else left = mid + 1;
        }
        cout << "Case #" << (t + 1) << ": " << left << " " << solve(left) << endl;
    }
    return 0;
}
