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

string N;
ll cache[42][10000];

const int primes[4] = {2, 3, 5, 7};

vector<int> getRem(int a, int b)
{
	vector<int> ret(4);
	FOR(i,a,b)
	{
		REP(j,4)
		{
			ret[j] = ret[j] * 10 + N[i] - '0';
			ret[j] %= primes[j];
		}
	}
	return ret;
}

vector<int> add(const vector<int>& a, const vector<int>& b)
{
	vector<int> ret(4);
	REP(i,4) ret[i] = (a[i]+b[i])%primes[i];
	return ret;
}

vector<int> sub(const vector<int>& a, const vector<int>& b)
{
	vector<int> ret(4);
	REP(i,4) ret[i] = (a[i]-b[i]+primes[i])%primes[i];
	return ret;
}

ll go(int here, const vector<int>& rem)
{
	if(here == N.sz) 
	{
		if(count(all(rem), 0) > 0) return 1;
		return 0;
	}
	int serial = 0;
	REP(i,4) serial = serial * 10 + rem[i];
	ll& ret = cache[here][serial];
	if(ret != -1) return ret;
	ret = 0;
	FOR(there,here+1,N.sz+1)
	{
		vector<int> seg = getRem(here, there);
		ret += go(there, add(rem, seg));
		ret += go(there, sub(rem, seg));
	}
	return ret;
}
int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		CLEAR(cache,-1);
		cin >> N;
		ll ret = 0;
		FOR(i,1,N.sz+1) 
			ret += go(i, getRem(0, i));
		printf("Case #%d: %Ld\n", cc+1, ret);
	}
}
