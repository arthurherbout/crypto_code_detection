#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cstdio>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include <list>
#include <cmath>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i,a,b) for(int i=(b)-1;i>=(a);--i)
#define REP(i,n) FOR(i,0,n)
#define REPD(i,n) FORD(i,0,n)
#define VAR(v,w) __typeof(w) v=(w)
#define FORE(it,c) for(VAR(it,(c).begin());it!=(c).end();++it)
#define PB push_back
#define ALL(c) (c).begin(),(c).end()
#define SIZE(c) ((int)(c).size())
#define MP make_pair
#define FT first
#define SD second
typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;
typedef vector<int> VI;
typedef vector<string> VS;
typedef vector<VI> VVI;
typedef pair<int,int> PII;
typedef vector<PII> VII;
typedef vector<double> VD;
typedef vector<LD> VLD;
typedef vector<LL> VLL;
typedef vector<bool> VB;
typedef istringstream ISS;
typedef ostringstream OSS;

int arr[50][50];

int main() {
	int ccc;
	cin >> ccc;
	REP(cc,ccc) {
		int n, m;
		cin >> n >> m;
		REP(i,n) REP(j,m)
			cin >> arr[i][j];
		VI thick;
		REP(i,n) {
			int mod = m % 3;
			int start = (mod == 1) ? 0 : 1;
			int t = 0;
			for (int j = start; j < m; j += 3)
				t += arr[i][j];
			thick.PB(t);
		}
		VI res(n,-1);
		int ii = 0;
		int last = 0;
		while (ii + 2 < n) {
			res[ii+2] = thick[ii+1] - thick[ii] + last;
			last = res[ii+2];
			ii += 3;
		}
		ii = 0;
		last = 0;
		while (ii + 2 < n) {
			res[n-1-(ii+2)] = thick[n-1-(ii+1)] - thick[n-1-ii] + last;
			last = res[n-1-(ii+2)];
			ii += 3;
		}
		int rr = -1;
		if (n % 3 == 2)
			rr = res[n>>1];
		else
			rr = thick[n>>1] - res[(n>>1)-1] - res[(n>>1)+1];
		cout << "Case #" << cc+1 << ": " << rr << endl;
	}
}
