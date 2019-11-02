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

int M, V;
int interiorNodes, leafNodes;
int G[10010];
int C[10010];
int mark[10010];
int cache[10010][2];
bool seen[10010][2];
VI f;

int calc(int root, int exp)
{
	if (root==2 && exp==1)
		root=2;
	int &res=cache[root][exp];
	if (seen[root][exp]) return res;
	res=-1;
	if (root>=interiorNodes)
	{
		if (exp==mark[root]) res=0;
	}
	else
	{
		int c1=root*2+1;
		int c2=root*2+2;
		int l, r;
		res=INF;
		if (G[root]==1) // AND
		{
			if (exp==1)
			{
				l=calc(c1, 1);
				r=calc(c2, 1);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				if (C[root]==1) // changeable
				{
					l=calc(c1, 1);
					r=calc(c2, 1);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
					l=calc(c1, 1);
					r=calc(c2, 0);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
					l=calc(c1, 0);
					r=calc(c2, 1);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
				}
			} else // (exp==0)
			{
				l=calc(c1, 0);
				r=calc(c2, 0);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				l=calc(c1, 1);
				r=calc(c2, 0);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				l=calc(c1, 0);
				r=calc(c2, 1);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				if (C[root]==1) // changeable
				{
					l=calc(c1, 0);
					r=calc(c2, 0);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
				}
			}
		}
		else // OR
		{
			if (exp==1)
			{
				l=calc(c1, 1);
				r=calc(c2, 1);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				l=calc(c1, 1);
				r=calc(c2, 0);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				l=calc(c1, 0);
				r=calc(c2, 1);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				if (C[root]==1) // changeable
				{
					l=calc(c1, 1);
					r=calc(c2, 1);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
				}
			} else // (exp==0)
			{
				l=calc(c1, 0);
				r=calc(c2, 0);
				if (l>=0 && r>=0) res=GMIN(res, l+r);
				if (C[root]==1) // changeable
				{
					l=calc(c1, 0);
					r=calc(c2, 0);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
					l=calc(c1, 1);
					r=calc(c2, 0);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
					l=calc(c1, 0);
					r=calc(c2, 1);
					if (l>=0 && r>=0) res=GMIN(res, l+r+1);
				}
			}
		}
		if (res>=INF) res=-1;
	}
	seen[root][exp]=true;
	return res;
}
 
int main()
{
	/*freopen("Round 2\\A\\sample.in", "r", stdin);
	freopen("Round 2\\A\\sample.out", "w", stdout);*/

	/*freopen("Round 2\\A\\A-small-attempt0.in", "r", stdin);
	freopen("Round 2\\A\\A-small-attempt0.out", "w", stdout);*/

	freopen("Round 2\\A\\A-large.in", "r", stdin);
	freopen("Round 2\\A\\A-large.out", "w", stdout);

	int numtest, test, i;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> M >> V;
		interiorNodes = (M-1)/2;
		leafNodes = (M+1)/2;
		RP(i, interiorNodes) cin >> G[i] >> C[i];
		FR(i, interiorNodes, M-1) cin >> mark[i];
		memset(cache, -1, sizeof(cache));
		memset(seen, false, sizeof(seen));
		int ans=calc(0, V);
		if (ans==-1) cout << "Case #" << (test+1) << ": IMPOSSIBLE" << endl;
		else cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
