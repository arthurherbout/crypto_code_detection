#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int main() {
	int C;
	scanf("%d", &C);
	for (int c = 0; c < C; c++) {
		int N;
		scanf("%d", &N);
		
		int g[3];
		for (int i = 0; i < N; i++)
			scanf("%d", &g[i]);
		
		int T;
		for (T = 100000000; ; T--)
			if (g[0] % T == g[1] % T && (N == 2 || g[0] % T == g[2] % T))
				break;
		
		int r = T - g[0] % T;
		if (r == T) r = 0;
		printf("Case #%d: %d\n", c + 1, r);
	}
}

