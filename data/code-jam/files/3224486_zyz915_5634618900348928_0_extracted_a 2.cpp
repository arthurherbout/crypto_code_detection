#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int N = 20020;
int dp[2][N];
char c[N], s[N];

inline int same(int i, int j) {
	return (c[i] == c[j] ? 2 : 1);
}

void run(int cas) {
	scanf("%s", c);
	int n = strlen(c), top = 0;
	for (int i = 0; i < n; i++) {
		s[++top] = c[i];
		if (top >= 2 && s[top] == s[top - 1])
			top -= 2;
	}
	printf("Case #%d: %d\n", cas, (n - top/2) * 5);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}

