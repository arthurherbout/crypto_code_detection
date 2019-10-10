#include <stdio.h>


int N;
int eles[1001];
int seen[1001];

void handle() {
	scanf ("%d ", &N);
	int i;
	for (i = 0; i < N; i++) {
		scanf ("%d ", &eles[i]);
		seen[i] = 0;
	}
	int total = 0;
	for (i = 0; i < N; i++) {
		if (!seen[i]) {
			int count = 1;
			int at = eles[i];
			seen[i] = 1;
			while (at != i) {
				seen[at] = 1;
				count++;
				at = eles[at];
			}
			if (count > total) {
				total = count;
			}
		}
	}
	int good = total <= N / 2;
	printf ("%s\n", good ? "GOOD" : "BAD");
}

int main() {
	int t;
	scanf ("%d ", &t);
	int i;
	for (i = 0; i < t; i++) {
		printf ("Case #%d: ", i + 1);
		handle();
	}
	return 0;
}