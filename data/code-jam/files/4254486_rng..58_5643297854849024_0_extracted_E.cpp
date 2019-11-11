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
#define _abs(x) ((x)>0?(x):-(x))

int N,D;
ll a[5010];

ll func(void){
	int i,j;
	
	REP(i,N) if(a[i] != a[i % (2*D)]) return -1;
	
	ll ans = 0;
	for(int d=1;d<=D;d*=2) REP(i,d){
		ll sum = 0;
		for(j=i+d-1;j<2*D;j+=2*d) sum += a[j];
		for(j=i+d;j<2*D;j+=2*d) sum -= a[j];
		if(sum % (D / d) != 0) return -1;
		ll tmp = sum / (D / d);
		ans += _abs(tmp);
		REP(j,2*D) if(j % (2*d) >= i && j % (2*d) < i+d) a[j] -= tmp;
	}
	
	REP(i,2*D) if(a[i] != a[0]) return -1;
	return ans;
}

void main2(void){
	int i;
	
	cin >> N >> D;
	REP(i,N) cin >> a[i];
	
	ll ans = func();
	if(ans == -1){
		cout << "CHEATERS!" << endl;
	} else {
		cout << ans << endl;
	}
}

int main(void){
	int TC,tc;
	cin >> TC;
	REP(tc,TC){
		printf("Case #%d: ", tc+1);
		main2();
	}
	return 0;
}
