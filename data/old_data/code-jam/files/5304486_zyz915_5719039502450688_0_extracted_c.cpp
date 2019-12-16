#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <vector>

using namespace std;

void run(int cas) {
	long long H, A, HX, AX, B, D;
	scanf("%lld%lld%lld%lld%lld%lld", &H, &A, &HX, &AX, &B, &D);
	if (A >= HX) {
		printf("Case #%d: 1\n", cas);
		return;
	}
	// b: buff
	int best_attack = 0x7fffffff;
	for (int b = 0; B * (b - 1) + A < HX; b++) {
		long long attack = A + B * b;
		long long rounds = b + (HX + attack - 1) / attack;
		best_attack = min(best_attack, (int) rounds);
		if (B == 0) break;
	}
	// d: debuff
	long long ans = -1;
	long long cur_health = H, cur_rounds = 0;
	while (ans == -1 || cur_rounds < ans) {
		long long health = cur_health, attack = AX, rounds = cur_rounds;
		//printf("=== health %lld, rounds %lld, attack %lld\n", health, rounds, attack);
		// without cure
		if (attack * (best_attack - 1) < health) {
			if (ans == -1 || rounds + best_attack < ans)
				ans = rounds + best_attack;
		}
		// with cure
		if (attack > 0 && attack * 2 < H) {
			int rem = (health - 1) / attack;
			int xu = (H - 1) / attack - 1;
			int cure = ((best_attack - 1) - rem + (xu - 1)) / xu;
			if (ans == -1 || rounds + best_attack + cure < ans) {
				//printf("health %lld, attack %lld, rounds %lld, cure %d, best %d\n",
				//		health, attack, rounds, cure, best_attack);
				ans = rounds + best_attack + cure;
			}
		}
		// debuff?
		if (attack == 0 || D == 0) break;
		long long newAX = max(0ll, AX - D);
		if (health <= newAX) {
			if (H - AX <= newAX) break; // no way!!
			cur_health = H - AX - newAX;
			cur_rounds += 2;
			AX = newAX;
		} else {
			cur_health -= newAX;
			cur_rounds += 1;
			AX = newAX;
		}
	}
	if (ans == -1)
		printf("Case #%d: IMPOSSIBLE\n", cas);
	else
		printf("Case #%d: %lld\n", cas, ans);
}

int main() {
	int cas, tt;
	scanf("%d", &tt);
	for (cas = 1; cas <= tt; cas++)
		run(cas);
	return 0;
}