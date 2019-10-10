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
 
#define RP(a,h) for(a=0; a<(h); a++)
#define FR(a,l,h) for(a=(l); a<=(h); a++)
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
#define LL long long

int n, M;
int X[100010], Y[100010];
LL cnt[3][3];
int wx0, wy0, wx1, wy1, wx2, wy2;

void process()
{
	memset(cnt, 0, sizeof(cnt));
	int i;
	RP(i, n) cnt[X[i]%3][Y[i]%3]++;
}

int main()
{
	freopen("Round 1B\\A-large.in", "r", stdin);
	freopen("Round 1B\\A-large.out", "w", stdout);
	LL numtest, test, i, j, k;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> n;
		LL A, B, C, D, x0, y0;
		cin >> A >> B >> C >> D >> x0 >> y0 >> M;
		X[0]=x0, Y[0]=y0;
		FR(i, 1, n-1)
		{
			X[i] = (A * X[i-1] + B) % M;
			Y[i] = (C * Y[i-1] + D) % M;
		}
		process();
		LL ans=0;
		RP(wx0, 3) RP(wy0, 3)
		RP(wx1, 3) RP(wy1, 3)
		{
			wx2=(6-wx0-wx1)%3;
			wy2=(6-wy0-wy1)%3;
			LL res0=cnt[wx0][wy0];
			LL res1=cnt[wx1][wy1];
			LL res2=cnt[wx2][wy2];

			if (res0==0 || res1==0 || res2==0) continue;

			if (wx1==wx0 && wy1==wy0) res1--;
			if (wx2==wx0 && wy2==wy0) res2--;
			if (wx2==wx1 && wy2==wy1) res2--;

			ans += res0*res1*res2;
		}
		ans/=6;
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
