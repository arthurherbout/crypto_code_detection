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

#define INF (1<<29)

int C;
int to[20][2], start[20][2], duration[20][2];
int cnt[20];

int func(int mask){
	int i;
	
	REP(i,C) cnt[i] = 0;
	
	int x = 0;
	int t = 0;
	
	REP(i,2*C){
		int id = cnt[x];
		if(id == 2) return INF;
		cnt[x]++;
		if(mask & (1 << x)) id ^= 1;
		int wait = (start[x][id] - t % 24 + 24) % 24;
		t += wait + duration[x][id];
		x = to[x][id];
	}
	
	return t;
}

void main2(void){
	int i,j;
	
	cin >> C;
	REP(i,C) REP(j,2){
		cin >> to[i][j];
		to[i][j]--;
		cin >> start[i][j];
		cin >> duration[i][j];
	}
	
	int ans = INF;
	int mask;
	REP(mask,(1<<C)){
		int tmp = func(mask);
		ans = min(ans, tmp);
	}
	
	cout << ans << endl;
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
