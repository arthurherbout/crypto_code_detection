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
	
int N;
ll x[160],y[160],z[160];
ll start[160],goal[160];
ll graph[160][160];

ll steps;
ll low[160],high[160];
ll low2[160],high2[160];

ll dist(int i, int j){
	return _abs(x[i] - x[j]) + _abs(y[i] - y[j]) + _abs(z[i] - z[j]);
}
	
void main2(void){
	int i,j;
	
	cin >> N;
	cin >> x[N] >> y[N] >> z[N];
	cin >> x[N+1] >> y[N+1] >> z[N+1];
	REP(i,N) cin >> x[i] >> y[i] >> z[i];
	
	REP(i,N) start[i] = dist(N, i);
	REP(i,N) goal[i] = dist(N+1, i);
	REP(i,N) REP(j,N) graph[i][j] = dist(i, j);
	
	steps = 1;
	REP(i,N) low[i] = high[i] = start[i];
	
	while(1){
		bool finished = false;
		REP(i,N) if(goal[i] >= low[i] && goal[i] <= high[i]) finished = true;
		if(finished) break;
		
		if(N == 1){
			cout << "IMPOSSIBLE" << endl;
			return;
		}
		
		steps++;
		
		REP(i,N){
			low2[i] = low[i];
			high2[i] = high[i];
			
			REP(j,N) if(j != i){
				ll L;
				if(graph[i][j] < low[j]){
					L = low[j] - graph[i][j];
				} else if(graph[i][j] > high[j]){
					L = graph[i][j] - high[j];
				} else {
					L = 0;
				}
				ll R = graph[i][j] + high[j];
				low2[i] = min(low2[i], L);
				high2[i] = max(high2[i], R);
			}
		}
		
		REP(i,N){
			low[i] = low2[i];
			high[i] = high2[i];
		}
		
	//	REP(i,N) cout << low[i] << ' ' << high[i] << ' ';
	//	cout << endl;
	}
	
	cout << steps << endl;
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
