#include <cstdio>

int main() {
	int t;
	scanf("%d\n", &t);
	for(int i = 1; i <= t; i++) {
		int K, C, S;
		scanf("%d %d %d\n", &K, &C, &S);
		printf("Case #%d:", i);
		for(int j = 1; j <= K; j++) {
            printf(" %d", j);
		}
		if(i < t) {
            printf("\n");
		}
	}
	return 0;
}
