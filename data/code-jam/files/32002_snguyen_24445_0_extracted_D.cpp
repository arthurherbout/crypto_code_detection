#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
 
using namespace std;

#define GMAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define GMIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define RP(a,h) for(a=0; a<(h); a++)
#define FR(a,l,h) for(a=(l); a<=(h); a++)
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
#define LL long long
#define INF 1000000

int MOD = 10007;

int cache[128][128];
int a[128][128];
int H, W, R;

int calc(int h, int w)
{
	if (h < 1 || w < 1) return 0;
	if (h == 1 && w == 1) return 1;
	int& res = cache[h][w];
	if (res>=0) return res;
	if (a[h][w]==1) return res=0;
	res=calc(h-2, w-1) + calc(h-1, w-2);
	res%=MOD;
	return res;
}

int main()
{
	freopen("Round 3\\D\\D-small-attempt0.in", "r", stdin);
	freopen("Round 3\\D\\D-small-attempt0.out", "w", stdout);

	int numtest, test, i, r, c;
	cin >> numtest;

	RP(test, numtest)
	{
		memset(cache, -1, sizeof(cache));
		memset(a, 0, sizeof(a));
		cin >> H >> W >> R;
		RP(i, R)
		{
			cin >> r >> c;
			a[r][c] = 1;
		}
		int ans = calc(H, W);
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
