#include <stdio.h>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

typedef long long ll;

int main() {

	int T;
	scanf("%d", &T);

	for (int tt = 1; tt <= T; tt++) {

		ll N, K;
		scanf("%lld %lld", &N, &K);
		ll MAX, MIN;

		priority_queue<pair<ll, pair<ll, ll> > >myQ;
		myQ.push({ N,{0,N + 1} });

		for (int i = 0; i < K; i++) {

			auto now = myQ.top();
			myQ.pop();

			ll left = now.second.first;
			ll right = now.second.second;
			ll mid = (left + right) / 2;

			MAX = right - mid - 1;
			MIN = mid - left - 1;

			myQ.push({ MAX,{mid,right} });
			myQ.push({ MIN,{left,mid} });
		}
		printf("Case #%d: %lld %lld\n", tt, MAX, MIN);
	}
}