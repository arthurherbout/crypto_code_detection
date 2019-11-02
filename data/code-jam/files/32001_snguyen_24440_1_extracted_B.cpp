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

#define GMAX(xx, yy) ((xx) > (yy) ? (xx) : (yy))
#define GMIN(xx, yy) ((xx) < (yy) ? (xx) : (yy))
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

int N, M, A;
int xx0, yy0, xx1, yy1, xx2, yy2;
bool found;

void process()
{
	xx0=yy0=0;
	found=false;
	if (N*M<A) return;
	for (xx2=N; xx2>=0; xx2--)
	{
		int need=xx2*M-A;
		for (yy1=M; yy1>=0; yy1--)
		{
			if (need==0)
			{
				xx1=yy2=0;
				found=true;
				return;
			}
			if (need<0) break;
			int a, b;
			for (a=1; a*a<=need; a++)
			{
				if (a>N && a>M) break;
				if (need%a==0)
				{
					b=need/a;
					if (a<=N && b<=M)
					{
						xx1=a; yy2=b; found=true; return;
					}
					if (a<=M && b<=N)
					{
						xx1=b; yy2=a; found=true; return;
					}
				}
			}
			need-=xx2;
		}
	}
	FR(xx1, 0, N) FR(yy1, 0, M)
		FR(xx2, xx1, N) FR(yy2, 0, M)
	{
		int v=xx2*yy1 - xx1*yy2;
		if (v<0) v=-v;
		if (v==A)
		{
			found=true;
			return;
		}
	}
}
 
int main()
{
	//freopen("Round 2\\B\\sample.in", "r", stdin);
	//freopen("Round 2\\B\\sample.out", "w", stdout);

	//freopen("Round 2\\B\\B-small-attempt0.in", "r", stdin);
	//freopen("Round 2\\B\\B-small-attempt0.out", "w", stdout);

	freopen("Round 2\\B\\B-large.in", "r", stdin);
	freopen("Round 2\\B\\B-large.out", "w", stdout);

	int numtest, test, i;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> N >> M >> A;
		found=false;
		process();
		if (!found) cout << "Case #" << (test+1) << ": IMPOSSIBLE" << endl;
		else cout << "Case #" << (test+1) << ": "
			<< xx0 << " " << yy0 << " "
			<< xx1 << " " << yy1 << " "
			<< xx2 << " " << yy2 << endl;
	}
	return 0;
}
