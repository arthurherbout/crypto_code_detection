#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

const int N = 1010;
char ch[N];
int id[N];

void run(int cas) {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf(" %c%d", &ch[i], &id[i]);
	// preprocessing
	int impossible = false;
	for (int j = n - 2; j >= 0; j--) {
		if (id[j] == 0) continue;
		int i = j + 1;
		while (i < n && id[i] != id[j]) i++;
		if (i == n) continue;
		if (ch[i] != ch[j]) continue;
		int k = i;
		while (k > j && (id[k] != 0 || ch[k] == ch[i])) k--;
		if (j == k) { //impossible
			impossible = true;
			break;
		}
		id[k] = -1;
		id[i] = -1;
	}
	if (impossible) {
		printf("Case #%d: CRIME TIME\n", cas);
		return;
	}
	for (int i = n - 1; i >= 0; i--) {
		if (id[i] <= 0) continue;
		if (ch[i] != 'L') continue;
		int j = i - 1;
		while (j >= 0 && id[j] != id[i]) j--;
		if (j < 0) continue;
		int i1 = i - 1;
		int j1 = j + 1;
		while (i1 > j && (ch[i1] == ch[i] || id[i1] != 0)) i1--;
		while (i > j1 && (ch[j] == ch[j1] || id[j1] != 0)) j1--;
		if (j1 == i || i1 == j) continue;
		id[j] = -1;
		id[i] = -1;
		id[j1] = -1;
		id[i1] = -1;
	}
	int mini = 0, men = 0;
	for (int i = 0; i < n; i++) {
		if (id[i] == -1) continue;
		if (ch[i] == 'E')
			men++;
		else
			men--;
		mini = min(mini, men);
	}
	int ans = men - mini;
	printf("Case #%d: %d\n", cas, ans);
	/*
	int mini = 0, masks = 0;
	memset(known, 0, sizeof(known));
	for (int i = 0; i < n; i++) {
		if (id[i] == -1) continue;
		if (ch[i] == 'E') {
			if (id[i] != 0 && known[id[i]] == 1) {
				puts("!!!!");
				exit(0);
			}
			if (id[i] > 0)
				known[id[i]] = 1;
			else
				masks += 1;
		}
		if (ch[i] == 'L') {
			if (id[i] && known[id[i]])
				known[id[i]] -= 1;
			else {
				masks -= 1;
				mini = min(mini, masks);
			}
		}
	}
	int ans = -mini;
	for (int i = 0; i < n; i++)
		if (id[i] != -1) {
			if (ch[i] == 'E') ans++;
			if (ch[i] == 'L') ans--;
		}
	printf("Case #%d: %d\n", cas, ans);
	*/
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}
