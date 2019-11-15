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
int X[200], Y[200];

int main()
{
	freopen("Round 1B\\A-small-attempt1.in", "r", stdin);
	freopen("Round 1B\\A-small-attempt1.out", "w", stdout);
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
		LL ans=0;
		RP(i, n) FR(j, i+1, n-1) FR(k, j+1, n-1)
		{
			if ((X[i]+X[j]+X[k])%3==0)
			if ((Y[i]+Y[j]+Y[k])%3==0)
				ans++;
		}
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
