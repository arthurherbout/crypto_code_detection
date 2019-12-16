// Korean programming contest community: http://algospot.com
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

#define CLEAR(x,with) memset(x,with,sizeof(x))
#define FOR(i,a,b) for(int (i) = (a); (i) < (b); ++(i))
#define REP(i,n) FOR(i,0,n)
#define pb push_back
#define sz size()
#define FORE(it,x) for(typeof(x.begin()) it = x.begin(); it != x.end(); ++it)
#define all(x) (x).begin(),(x).end()

typedef long long ll;

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		int p, k, l;
		cin >> p >> k >> l;
		vector<int> presses(l);
		REP(i,l) cin >> presses[i];
		sort(all(presses));
		reverse(all(presses));
		cout << "Case #" << cc+1 << ": ";
		if(l > p*k)
			cout << "Impossible" << endl;
		else
		{
			int ret = 0;
			REP(i,l) ret += (i/k+1)*presses[i];
			cout << ret << endl;
		}
		
	}
}
