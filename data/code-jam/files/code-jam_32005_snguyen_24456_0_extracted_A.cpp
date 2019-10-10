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
const int MAX = 1000010;
int ans;
int minH, maxH, minW, maxW;
int n;
int canbH[MAX], canbW[MAX];
int H[2000], W[2000];
string X[2000];

void process_bird()
{
	int i;
	RP(i, n) if (X[i]=="BIRD")
	{
		if (minH==-1 || minH>H[i]) minH=H[i];
		if (maxH==-1 || maxH<H[i]) maxH=H[i];
		if (minW==-1 || minW>W[i]) minW=W[i];
		if (maxW==-1 || maxW<W[i]) maxW=W[i];
	}
}

void process_notbird()
{
	int i, j;
	RP(i, n) if (X[i]=="NOT")
	{
		if (minH!=-1 && H[i]>=minH && H[i]<=maxH)
		{
			if (minW==-1) canbW[W[i]]=0;
			else {
				if (W[i]<minW) FR(j, 1, W[i]) canbW[j]=0;
				if (W[i]>maxW) FR(j, W[i], MAX-1) canbW[j]=0;
			}
		}

		if (minW!=-1 && W[i]>=minW && W[i]<=maxW)
		{
			if (minH==-1) canbH[H[i]]=0;
			else {
				if (H[i]<minH) FR(j, 1, H[i]) canbH[j]=0;
				if (H[i]>maxH) FR(j, H[i], MAX-1) canbH[j]=0;
			}
		}
	}
}

void process()
{
	memset(canbH, -1, sizeof(canbH));
	memset(canbW, -1, sizeof(canbW));
	minH=minW = -1;
	maxH=maxW = -1;
	cin >> n;
	int i;
	RP(i, n)
	{
		int h, w;
		string x;
		cin >> h >> w >> x;
		if (x=="NOT") { cin>>x; x="NOT"; }
		H[i] = h;
		W[i] = w;
		X[i] = x;
	}
	process_bird();
	process_notbird();
}

void query()
{
	int h, w;

	cin >> h >> w;

	if (canbH[h]==0) { cout << "NOT BIRD" << endl; return; }
	if (canbW[w]==0) { cout << "NOT BIRD" << endl; return; }

	int i, j;
	RP(i, n) if (X[i]=="BIRD")
	{
		if (H[i]==h && W[i]==w) { cout << "BIRD" << endl; return; }
	}
	RP(i, n) if (X[i]=="BIRD")
	FR(j, i+1, n-1) if (X[j]=="BIRD")
	{
		int h1=GMIN(H[i], H[j]);
		int h2=GMAX(H[i], H[j]);
		int w1=GMIN(W[i], W[j]);
		int w2=GMAX(W[i], W[j]);
		if (h>=h1 && h<=h2 && w>=w1 && w<=w2) { cout << "BIRD" << endl; return; }
	}

	cout << "UNKNOWN" << endl;
}

int main()
{
	//freopen("Round 4\\A\\sampleA.in", "r", stdin);
	//freopen("Round 4\\A\\sampleA.out", "w", stdout);
	
	freopen("Round 4\\A\\A-small-attempt1.in", "r", stdin);
	freopen("Round 4\\A\\A-small-attempt1.out", "w", stdout);

	//freopen("Round 4\\A\\A-large.in", "r", stdin);
	//freopen("Round 4\\A\\A-large.out", "w", stdout);

	int numtest, test;
	cin >> numtest;

	RP(test, numtest)
	{
		ans=0;
		process();
		cout << "Case #" << (test+1) << ":" << endl;
		int m, i;
		cin >> m;
		RP(i, m)		query();
		//cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
