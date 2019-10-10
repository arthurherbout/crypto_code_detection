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

#define ABS(x) ((x) > 0 ? (x) : -(x))
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
const double PI = cos(-1.0);
int hx[4]={-1, 0, 1, 0};
int hy[4]={0, 1, 0, -1};
/*-----------------------------------------------------------------------------------------------------------------------------*/
const int MAX = 110;
int A[MAX][MAX], B[MAX][MAX];
int n, m;
string ans;
bool ex[MAX];
int walk[MAX];

void check_it()
{
	int i, j;
	FR(i, 1, m) FR(j, 1, m) if (B[i][j]==1)
		if (A[walk[i]][walk[j]]==0) return;
	ans="YES";
}

void doit(int v)
{
	if (ans=="YES") return;
	if (v==m+1)
	{
		check_it();
		return;
	}
	int i;
	FR(i, 1, n) if (!ex[i])
	{
		ex[i]=true;
		walk[v]=i;
		doit(v+1);
		ex[i]=false;
	}
}

void process()
{
	memset(A, 0, sizeof(A));
	memset(B, 0, sizeof(B));
	memset(ex, false, sizeof(ex));
	memset(walk, 0, sizeof(walk));
	cin >> n;
	int i, x, y;
	RP(i, n-1)
	{
		cin >> x >> y;
		A[x][y]=A[y][x]=1;
	}
	cin >> m;
	RP(i, m-1)
	{
		cin >> x >> y;
		B[x][y]=B[y][x]=1;
	}
	ans="NO";
	doit(1);
}

int main()
{
	//freopen("Round 4\\D\\sampleD.in", "r", stdin);
	//freopen("Round 4\\D\\sampleD.out", "w", stdout);
	
	freopen("Round 4\\D\\D-small-attempt0.in", "r", stdin);
	freopen("Round 4\\D\\D-small-attempt0.out", "w", stdout);

	//freopen("Round 4\\D\\D-large.in", "r", stdin);
	//freopen("Round 4\\D\\D-large.out", "w", stdout);

	int numtest, test;
	cin >> numtest;

	RP(test, numtest)
	{
		ans="NO";
		process();
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
