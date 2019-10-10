#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<utility>
#include<set>
#include<map>
#include<queue>
#include<vector>
#include <string>
//cl3488
#define RI(X) scanf("%d", &(X))
#define RII(X, Y) scanf("%d%d", &(X), &(Y))
#define RIII(X, Y, Z) scanf("%d%d%d", &(X), &(Y), &(Z))
#define DRI(X) int (X); scanf("%d", &X)
#define DRII(X, Y) int X, Y; scanf("%d%d", &X, &Y)
#define DRIII(X, Y, Z) int X, Y, Z; scanf("%d%d%d", &X, &Y, &Z)
#define println(X) printf("%d\n",(X))
#define PB push_back
#define MP make_pair
using namespace std;
typedef long long ll;
ll delay[1000];
int B;
ll numProcessedAfter(ll T){
	if (T < 0) return 0;
	//cout << T << "!\n";
	ll ans = 0;
	for (int i = 0; i < B; i++){
		ans += T / delay[i] + 1;
	}
	return ans;
}
int main(){
	freopen("haircut.in", "r", stdin);
	freopen("haircut.out", "w", stdout);
	DRI(T);
	for (int tc = 1; tc <= T; tc++){
		cout << "Case #" << tc << ": ";
		ll N;
		cin >> B >> N;
		for (int b = 0; b < B; b++){
			cin >> delay[b];
		}
		ll lo = -1;
		ll hi = 100000 * (N+1);
		while (hi - lo > 1){
			ll mid = (hi + lo) / 2;
			if (numProcessedAfter(mid) >= N){
				hi = mid;
			}
			else{
				lo = mid;
			}
		}
		ll T = hi;
		vector<int> availableAtCriticalPoint;
		for (int i = 0; i < B; i++){
			if (T % delay[i]  ==  0){
				availableAtCriticalPoint.push_back(i);
			}
		}
		//cout << N - 1 - numProcessedAfter(T - 1) << "!!  ";
		cout << availableAtCriticalPoint[N-1 - numProcessedAfter(T - 1)] + 1 << "\n";
	}
	return 0;
}