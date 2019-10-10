#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <stack>
#include <queue>
#include <map>

using namespace std;

int bitCount(int t){
	int res = 0;
	for(int i=t;i;i&=i-1) res++;
	return res;
}

int solveSmall(const vector< pair<int,int> >& ev){
	vector<int> id;
	for(int i=0;i<ev.size();i++){
		if(ev[i].second != 0) id.push_back(ev[i].second);
	}
	sort(id.begin(), id.end());
	id.erase(unique(id.begin(), id.end()), id.end());
	static int dp[2][1<<15];
	for(int i=0;i<(1<<15);i++) dp[0][i] = 1;
	for(int e=0;e<ev.size();e++){
		int cur = e%2, next = 1-cur;
		memset(dp[next], 0, sizeof(dp[next]));
		int checkID = -1;
		for(int i=0;i<id.size();i++){
			if(id[i] == ev[e].second) checkID = i;
		}
		for(int i=0;i<(1<<15);i++){
			if(!dp[cur][i]) continue;
			if(checkID == -1){
				for(int j=0;j<15;j++){
					if(ev[e].first == 0){
						if(!(i&(1<<j))) dp[next][i|(1<<j)] = 1;
					} else {
						if(i&(1<<j)) dp[next][i^(1<<j)] = 1;
					}
				}
			} else {
				if(ev[e].first == 0){
					if(!(i&(1<<checkID))) dp[next][i|(1<<checkID)] = 1;
				} else {
					if(i&(1<<checkID)) dp[next][i^(1<<checkID)] = 1;
				}
			}
		}
	}
	int res = 100;
	for(int i=0;i<(1<<15);i++){
		if(dp[ev.size()%2][i]) res = min(bitCount(i), res);
	}
	return res;
}

int main(){
	int T; cin >> T;
	for(int test=1;test<=T;test++){
		int N; cin >> N;
		vector< pair<int,int> > ev(N);
		for(int i=0;i<N;i++){
			char c; int id; cin >> c >> id;
			if(c == 'E') ev[i] = make_pair(0, id);
			else         ev[i] = make_pair(1, id);
		}
		int res = solveSmall(ev);
		if(res == 100) printf("Case #%d: CRIME TIME\n", test);
		else printf("Case #%d: %d\n", test, res);
	}
}
