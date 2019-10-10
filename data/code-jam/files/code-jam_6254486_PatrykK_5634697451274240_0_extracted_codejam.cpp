#include <cstdio>

int main() {
	int t;
	scanf("%d\n", &t);
	for(int i = 1; i <= t; i++) {
		char *S = new char[101];
		int *tab = new int[100];
		int size = 0;
		tab[size] = 0;
		gets(S);
		char last = S[0];
		for(int j = 0; S[j] > 0; j++) {
			if(S[j] == last) {
				if(S[j] == '+') {
					++tab[size];
				}
				else {
					--tab[size];
				}
			}
			else {
				last = S[j];
				tab[++size] = S[j] == '+' ? 1 : -1;
			}
		}
		++size;
		printf("Case #%d: %d\n", i, size - (tab[size - 1] > 0));
	}
	return 0;
}
