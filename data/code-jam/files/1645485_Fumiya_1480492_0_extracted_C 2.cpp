#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <algorithm>

using namespace std;

const double EPS = 1e-8;

bool ithBit(int t, int i){
	return t&(1<<i);
}

bool valid(const vector<double> &speed, const vector<double> &pos, int side){
	int n = speed.size();
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			if(i==j) continue;
			if(ithBit(side, i) == ithBit(side, j)){
				if(abs(pos[i]-pos[j]) < 5.0-EPS) return false;
				if(speed[i] < speed[j] && abs(pos[i]-(pos[j]+5)) < EPS) return false;
				if(speed[j] < speed[i] && abs(pos[j]-(pos[i]+5)) < EPS) return false;
			}
		}
	}
	return true;
}

int main(){
	int T; cin >> T;
	int side;
	bool ok[2][1<<6];
	for(int test=1;test<=T;test++){
		int N; cin >> N;
		vector<double> S(N), P(N), pos(N);
		char c;
		for(int i=0;i<N;i++){
			cin >> c >> S[i] >> P[i];
			if(c=='R') side |= (1<<i);
		}
		vector<double> time;
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){
				if(abs(S[i]-S[j]) < 1e-8) continue;
				if(S[i] < S[j]) continue;
				if(P[j] >= P[i]+5)
					time.push_back((P[j]-(P[i]+5))/(S[i]-S[j]));
				if(P[j]+5 >= P[i])
					time.push_back((P[j]+5-P[i])/(S[i]-S[j]));
			}
		}
		sort(time.begin(), time.end());

		memset(ok, false, sizeof(ok));

		int noChg = 0;
		for(int j=0;j<N;j++){
			for(int k=j+1;k<N;k++){
				if(abs(P[j]-P[k]) < 5.0-EPS) noChg |= (1<<j)|(1<<k);
			}
		}
		for(int j=0;j<(1<<N);j++){
			if((j&noChg) != (side&noChg)) continue;
			if(valid(S, P, j)) ok[0][j] = true;
		}

		int cur = 0;
		double res = -1;
		
		for(int i=0;i<time.size();i++){
			for(int j=0;j<N;j++)
				pos[j] = P[j] + time[i]*S[j];
			int noChg = 0;
			for(int j=0;j<N;j++){
				for(int k=j+1;k<N;k++){
					if(abs(pos[j]-pos[k]) < 5.0-EPS) noChg |= (1<<j)|(1<<k);
				}
			}
			memset(ok[1-cur], false, sizeof(ok[1-cur]));
			for(int j=0;j<(1<<N);j++){
				if(!ok[cur][j]) continue;
				for(int k=0;k<(1<<N);k++){
					if((j&noChg) != (k&noChg)) continue;
					if(valid(S, pos, k)) ok[1-cur][k] = true;
				}
			}
			bool con = false;
			for(int j=0;j<(1<<N);j++)
				if(ok[1-cur][j]) con = true;
			if(!con){
				res = time[i];
				break;
			}
			cur = 1-cur;
		}
		if(res > -EPS)
			printf("Case #%d: %.8lf\n", test, res);
		else
			printf("Case #%d: Possible\n", test);
	}
}