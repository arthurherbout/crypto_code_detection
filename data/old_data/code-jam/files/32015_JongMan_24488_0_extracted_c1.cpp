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
const ll M = 1000000007;
int n, m;
ll A[100], X, Y, Z;
vector<ll> limits;
ll cache[1001];

ll go(int here)
{
	ll& ret = cache[here];
	if(ret != -1) return ret;
	ret = 1;
	FOR(there,here+1,n) if(limits[there] > limits[here]) 
	{
		ret += go(there);
		ret %= M;
	}
	return ret;
}

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		cin >> n >> m >> X >> Y >> Z;
		REP(i,m) cin >> A[i];
		limits.resize(n);
		REP(i,n) 
		{
			limits[i] = A[i%m];
			A[i%m] = (X * A[i%m] + Y * (i+1)) % Z;
		}
		CLEAR(cache,-1);
		ll ret = 0;
		REP(i,n) 
		{
			ret += go(i);
			ret %= M;
		}
		printf("Case #%d: %Ld\n", cc+1, ret);
	}
}
