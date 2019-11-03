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
#define ABS(X) (((X) > 0) ? (X) : -(X))
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

int k;
int ans;
string S;
VI tt;

string compress()
{
	string ret="";
	int i;
	int be=0;
	while (be<SZ(S))
	{
		RP(i, k) ret+=S[be+tt[i]];
		be+=k;
	}
	return ret;
}

void process()
{
	ans=10000;
	tt.clear();
	int i;
	RP(i, k) tt.pb(i);
	do {
		string s1=compress();
		int cnt=1;
		FR(i, 1, SZ(s1)-1) if (s1[i]!=s1[i-1]) cnt++;
		ans=GMIN(ans, cnt);
	} while (next_permutation(ALL(tt)));
}
 
int main()
{
	//freopen("Round 2\\D\\sample.in", "r", stdin);
	//freopen("Round 2\\D\\sample.out", "w", stdout);

	freopen("Round 2\\D\\D-small-attempt0.in", "r", stdin);
	freopen("Round 2\\D\\D-small-attempt0.out", "w", stdout);

	int numtest, test, i;
	cin >> numtest;
	RP(test, numtest)
	{
		cin >> k;
		cin >> S;
		process();
		cout << "Case #" << (test+1) << ": " << ans << endl;
	}
	return 0;
}
