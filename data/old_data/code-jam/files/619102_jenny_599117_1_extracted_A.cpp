#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

int n, pova[1000], povb[1000];

inline void task() {
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf("%d %d", pova + i, povb + i);
		
	int r = 0;
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
			if ((pova[i] > pova[j] && povb[i] < povb[j]) || (pova[i] < pova[j] && povb[i] > povb[j]))
				r++;
	printf("%d\n", r);
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 1; i <= t; i++) {
		printf("Case #%d: ", i);
		task();
	}
}

