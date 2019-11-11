#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solveSmall(){
	int c[80], s[80], t[80];
	int N; cin >> N;
	for(int i=0;i<N;i++) cin >> c[i] >> s[i] >> t[i];
	int M; cin >> M;
	for(int i=N;i<N+M;i++) cin >> c[i] >> s[i] >> t[i];
	bool used[80];
	memset(used, false, sizeof(used));
	int open = N;
	int score = 0, res = 0;
	int turn = 1;
	while(true){
		for(int i=0;i<open;i++){
			if(used[i]) continue;
			if(t[i] > 0){
				open = min(N+M, open+c[i]);
				score += s[i];
				turn += t[i]-1;
				used[i] = true;
			}
		}
		vector<int> vs;
		for(int i=0;i<open;i++){
			if(used[i]) continue;
			vs.push_back(-s[i]);
		}
		if(vs.empty()) break;
		sort(vs.begin(), vs.end());
		int sum = 0;
		for(int i=0;i<turn&&i<vs.size();i++){
			sum -= vs[i];
		}
		res = max(res, score+sum);

		vector< pair<int,int> > vp;
		for(int i=0;i<open;i++){
			if(used[i]) continue;
			if(c[i]==1)
				vp.push_back(make_pair(-s[i],i));
		}
		if(vp.empty()) break;
		sort(vp.begin(), vp.end());
		open = min(N+M, open+c[vp[0].second]);
		score -= vp[0].first;
		used[vp[0].second] = true;
		turn--;
		if(turn == 0) break;
	}
	return max(score, res);
}

int main(){
	int TEST; cin >> TEST;
	for(int test=1;test<=TEST;test++){
		printf("Case #%d: %d\n", test, solveSmall());
	}
}