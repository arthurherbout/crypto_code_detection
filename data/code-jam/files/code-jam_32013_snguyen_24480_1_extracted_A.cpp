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
#define GMAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define GMIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define SZ(a) (LL)a.size()
#define ALL(a) a.begin(), a.end()
#define pb push_back
typedef vector <int> VI;
typedef vector <string> VS;
typedef pair<int, int> PII;
#define LL long long

const int INF = 100000000;

int s, q;
VS engines, queries;
int ans;
int d[105][1005];

void process()
{
	if (q==0) { ans=0; return; }

	memset(d, 0, sizeof(d));
	int i, j, k;
	RP(i, s) if (engines[i] == queries[0]) d[i][0] = INF; else d[i][0] = 0;

	FR(j, 1, q-1)
	{
		RP(i, s)
		{
			d[i][j] = INF;
			if (engines[i] != queries[j])
			{
				RP(k, s)
				{
					int nv = d[k][j-1];
					if (k != i) nv++;
					
					d[i][j] = GMIN(d[i][j], nv);
				}
			}
		}
	}

	ans = INF;
	RP(i, s) ans = GMIN(ans, d[i][q-1]);
}

string str;

void getLine()
{
	char cc[1000];
	cin.getline(cc, 1000);

	str = "";
	int i;
	RP(i, strlen(cc)) str += cc[i];
}
 
int main()
{
	freopen(".\\A\\A-large.in", "r", stdin);
	freopen(".\\A\\A-large.out", "w", stdout);
	LL tc, i, j;
	cin >> tc;
	RP(i, tc)
	{
		engines.clear();
		queries.clear();
		cin >> s; getLine();
		RP(j, s) { getLine(); engines.pb(str); }
		cin >> q; getLine();
		RP(j, q) { getLine(); queries.pb(str); }
		process();
		cout << "Case #" << (i+1) << ": " << ans << endl;
	}
	return 0;
}
