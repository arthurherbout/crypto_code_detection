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
bool graph[30][30];

int n;
double a[30][30];

int count_mst(void){
	int i,j,k;
	
	REP(i,N) REP(j,N) a[i][j] = 0.0;
	REP(i,N) REP(j,N) if(graph[i][j]){
		a[i][j]--;
		a[i][i]++;
	}
	
	n = N - 1;
	REP(j,n){
		for(i=j;i<n;i++) if(abs(a[i][j]) > 0.1) break;
		if(i == n) return 0;
		REP(k,n) swap(a[i][k], a[j][k]);
		for(i=j+1;i<n;i++){
			double coef = -a[i][j] / a[j][j];
			REP(k,n) a[i][k] += coef * a[j][k];
		}
	}
	
	double ans = 1.0;
	REP(i,n) ans *= a[i][i];
	if(ans > 1.0E+9) return 1000000000;
	return (int)(ans + 0.5);
}

int K;
	
void main2(void){
	int i,j;
	
	cin >> K;
//	K = rand() % 10000 + 3;
	
	N = 22;
	REP(i,N) REP(j,N) graph[i][j] = false;
	
	if(K == 22){
		REP(i,N) REP(j,N) graph[i][j] = (abs(i-j) == 1);
		graph[0][N-1] = graph[N-1][0] = true;
	} else {
	
	while(1){
		int tmp = count_mst();
	//	cerr << tmp << ' ';
		
		if(tmp == K) break;
		
		while(1){
			int x = rand() % N;
			int y = rand() % N;
			if(x != y){
				if(graph[x][y] && tmp > K){
					graph[x][y] = graph[y][x] = false;
					break;
				}
				if(!graph[x][y] && tmp < K){
					graph[x][y] = graph[y][x] = true;
					break;
				}
			}
		}
	}
	
	}
	
	cout << N << endl;
	REP(i,N){
		REP(j,N) cout << graph[i][j];
		cout << endl;
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
