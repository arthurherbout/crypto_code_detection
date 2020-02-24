#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <map>
#include <vector>
#include <math.h>
#include <assert.h>
using namespace std;
typedef long long ll;

#if 1
#define PN 36
int _p[PN], _q, _r;
void pinit() { signal(SIGCHLD, SIG_IGN); }
void pblock(int p = _p[_q]) { while (p && !kill(p, 0)) usleep(1e4L); }
int pfork() {
	_r = (_q + 1) % PN; pblock(_p[_r]);
	if (_p[_r] = fork()) return _q = _r, 1; return 0;
}
void pexit() { fflush(stdout); exit(0); }
#else
void pinit() {}
void pblock(int p = 0) {};
int pfork() { return 0; }
void pexit() {}
#endif

const ll INF = (ll)1e18 + 1;
#define foreach(i,a) for(__typeof((a).begin())i=(a).begin();i!=(a).end();i++)
#define foreach_rev(i,a) for(__typeof((a).rbegin())i=(a).rbegin();i!=(a).rend();i++)


ll M, F, N;  // money, delivery fee, # of foods
ll P[256], S[256];  // cost, stale

void input() {
	scanf("%lld%lld%lld", &M, &F, &N);
	for (int i = 0; i < N; i++) scanf("%lld%lld", &P[i], &S[i]);
}

map<ll, ll> stales;

ll test(ll delivery) {
	if (M / F < delivery) return 0;
	ll money = M - delivery * F;
	ll last_day = -1;
	foreach (i, stales) {
		//printf("money: %lld, day: %lld-%lld, cost: %lld\n", money, last_day, i->first, i->second);
//		if ((j->first - i->first) * delivery * i->second >= money) {
		if (i->second == 0 ||
		    (i->first - last_day) <= money / delivery / i->second) {
			money -= (i->first - last_day) * delivery * i->second;
			assert(0 <= money);
		} else {
			// return (i->first + 1) * delivery + money / i->second;
			return (last_day + 1) * delivery + money / i->second;
		}
		//printf("money: %lld\n", money);
		//last_cost = i->second;
		last_day = i->first;
	}
}

ll search(ll l = 1, ll r = INF) {
	r = min(r, M / F);
	while (l + 3 <= r) {
		ll a = (2 * l + r) / 3, b = (l + 2 * r) / 3;
		ll result_a = test(a), result_b = test(b);
		if (result_a < result_b) {
			l = a;
		} else if (result_a > result_b) {
			r = b;
		} else {
			break;
			//return max(search(l, a), max(search(a + 1, b - 1), search(b, r)));
		}
	}
	//ll l = 1, r = 100;
	//l = 1; r = 10000000;
	ll max_key = l, max_val = 0;
	for (ll i = l; i <= r; i++) {
		ll result = test(i);
		//printf("%lld: %lld\n", i, result);
		if (max_val < result) {
			max_val = result;
			max_key = i;
		}
	}
	return max_val;
}

void solve(int case_x) {
	//stales[-1] = 0;
	stales[INF] = INF;
	for (int i = 0; i < N; i++) {
		if (stales.find(S[i]) == stales.end()) stales[S[i]] = INF;
		stales[S[i]] = min(stales[S[i]], P[i]);
	}
	ll min_cost = INF;
	foreach_rev(i, stales) {
		min_cost = i->second = min(i->second, min_cost);
		//printf("%lld: %lld\n", i->first, min_cost);
	}
	ll ans = search();
	pblock();
	printf("Case #%d: %lld\n", case_x, ans);
}

int main() {
	int case_number;
	scanf("%d", &case_number);
	pinit();
	for (int case_x = 1; case_x <= case_number; case_x++) {
		input();
		if (pfork()) continue;
		solve(case_x);
		pexit();
	}
	pblock();
	return 0;
}
