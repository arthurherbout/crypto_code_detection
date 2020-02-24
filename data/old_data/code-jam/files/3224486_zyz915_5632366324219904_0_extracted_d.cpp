#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int N = 220;
char buf[N], ans0[N], ans1[N], s[N];

void run(int cas) {
	int n, l;
	scanf("%d%d", &n, &l);
	memset(buf, '1', sizeof(buf));
	buf[l] = 0; // consists entirely of 1s
	int poss = true;
	for (int i = 0; i < n; i++) {
		scanf(" %s", s);
		if (strcmp(s, buf) == 0) // contain same string?
			poss = false;
	}
	scanf(" %s", buf); // read B
	for (int i = 0; i < l; i++) {
		ans0[i * 2] = '0';
		ans0[i * 2 + 1] = '?';
		ans1[i] = '1';
	}
	ans0[l * 2] = 0; // consists of l (0?)s
	ans1[l - 1] = 0; // consists of (l-1) 1s
	if (poss)
		printf("Case #%d: %s %s\n", cas, ans0, ans1);
	else
		printf("Case #%d: IMPOSSIBLE\n", cas);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}

