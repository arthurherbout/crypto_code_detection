#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

typedef long long ll;
#define MOD 1000000007ll
#define INF (1ll<<60)

int X,Y;
ll a[210][210];

int N;
int x[210],y[210];
int val[210];

void main2(void){
	int i,j,k;
	
	ll D;
	cin >> X >> Y >> N >> D;
	REP(i,N){
		cin >> x[i] >> y[i] >> val[i];
		x[i]--; y[i]--;
	}
	
	REP(i,X) REP(j,Y) a[i][j] = INF;
	REP(i,X) REP(j,Y) REP(k,N){
		ll tmp = abs(x[k] - i) + abs(y[k] - j);
		a[i][j] = min(a[i][j], val[k] + D * tmp);
	}
	
	bool good = true;
	REP(i,N) if(a[x[i]][y[i]] != val[i]) good = false;
	
	if(good){
		ll sum = 0;
		REP(i,X) REP(j,Y) sum = (sum + a[i][j] % MOD) % MOD;
		cout << sum << endl;
	} else {
		cout << "IMPOSSIBLE" << endl;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){
	int TC,tc;
	cin >> TC;
	REP(tc,TC){
		printf("Case #%d: ", tc + 1);
		main2();
	}
	return 0;
}
