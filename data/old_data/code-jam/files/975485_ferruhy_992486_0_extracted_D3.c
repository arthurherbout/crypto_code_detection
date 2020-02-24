#include <stdio.h>
#include <string.h>

#define LEN 1001

int A[LEN];
int M[LEN];

int main(int argc, char *argv[]) {
	int T;
	int N;
	int i, j, k, tmp;
	int x;
	float val;

	scanf("%d", &T);
	//printf("T:%d\n", T);
	for (i = 0; i < T; i++) {
		scanf("%d", &N);
		//printf("N:%d\n", N);
		for (j = 1; j <= N; j++) {
			scanf("%d", &A[j]);
		}

		j = 1;
		val = 0;
		memset(M, 0, LEN);
		while (j <= N) {
			if (M[j] == 0) {
				if (A[j] == j) {
					M[j] = 1;
					j++;
					continue;
				}

				k = A[j]; x = 0;
				M[j] = 1;
				//printf("k: %d,", k);
				while (k != j) {
					M[k] = 1;
					k = A[k];
					//printf("%d,", k);
					x++;
				}
				//printf(". x:%d ", x);
				val += (x + 1);
				//printf("v:%f \n", val);
			}
			j++;
		}

		printf("Case #%u: %f\n", i+1, val);
	}

	return 0;
}
