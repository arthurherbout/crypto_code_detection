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

VS a;
int cache[12][1<<12];
int H, W;
int all;

int calc(int row, int mask)
{
	int &res = cache[row][mask];
	if (res!=-1) return res;
	int i, abv, cnt=0;
	RP(i, W) if ((1<<i)&mask)
	{
		if (a[row][i]=='x') return res=-2;
		if (i>0 && ((1<<(i-1))&mask)) return res=-2;
		if (i<W-1 && ((1<<(i+1))&mask)) return res=-2;
		cnt++;
	}
	if (row==0) return res=cnt;

	res=cnt;
	RP(abv, all)
	{
		int r0=calc(row-1, abv);
		if (r0>0)
		{
			bool ok=true;
			RP(i, W) if ((1<<i)&abv)
			{
				if (i>0 && ((1<<(i-1))&mask)) {ok=false; break;}
				if (i<W-1 && ((1<<(i+1))&mask)) {ok=false; break;}
			}
			if (ok) res=GMAX(res, cnt+r0);
		}
	}
	return res;
}
 
int main()
{
	//freopen("Round 3\\C\\sample.in", "r", stdin);
	//freopen("Round 3\\C\\sample.out", "w", stdout);

	freopen("Round 3\\C\\C-small-attempt2.in", "r", stdin);
	freopen("Round 3\\C\\C-small-attempt2.out", "w", stdout);

	int numtest, test, i;
	cin >> numtest;

	RP(test, numtest)
	{
		memset(cache, -1, sizeof(cache));
		cin >> H >> W;
		a.resize(H);
		RP(i, H)
		{
			cin >> a[i];
		}
		all = (1<<W);
		int ans = 0;
		RP(i, all)
		{
			int res=calc(H-1, i);
			if (res>=0) ans = GMAX(ans, res);
		}
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
