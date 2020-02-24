#include <stdio.h>
#include <vector>
#include <iostream>
using namespace std;

int main() {
	//freopen("C-small-attempt2.in", "r", stdin);
	//freopen("C-small-attempt2.out", "w", stdout);
	int ca;
	int r, c, m;
	scanf("%d", &ca);
	for (int t = 0; t < ca; t++) {
		printf("Case #%d:\n", t + 1);
		scanf("%d%d%d", &r, &c, &m);
		vector<vector<char> > ret(r, vector<char>(c, 'X'));
		//printf("%d %d %d\n", r, c, m);
		int finish = 0;
		int total = r * c;
		if (r == 1 && c == 1) {
			ret[0][0] = 'c';
		} else if (r == 1 || c == 1) {
			if (m > r * c - 2) {
				printf("Impossible\n");
				finish = 1;
			} else if (r == 1) {
				ret[0][0] = 'c';
				ret[0][1] = '.';
				int cur = 2;
				for (int j = 2; j < c; j++) {
					if (cur < total - m) {
						ret[0][j] = '.';
						cur++;
					} else
						ret[0][j] = '*';
				}
			} else {
				ret[0][0] = 'c';
				ret[1][0] = '.';
				int cur = 2;
				for (int j = 2; j < r; j++) {
					if (cur < total - m) {
						ret[j][0] = '.';
						cur++;
					} else
						ret[j][0] = '*';
				}
			}

		} else {
			if (m > r * c - 4) {
				printf("Impossible\n");
				finish = 1;
			} else {
				ret[0][0] = 'c';
				ret[0][1] = '.';
				ret[1][0] = '.';
				ret[1][1] = '.';
				int cur = 4;
				total = total - m;
				for (int i = 0; i < r; i++) {
					for (int j = 0; j < c; j++) {
						if (ret[i][j] != 'X')
							continue;
						if (cur < total) {
							ret[i][j] = '.';
							cur++;
						} else
							ret[i][j] = '*';
					}
				}
			}
		}
		if (finish == 0) {
			for (int i = 0; i < r; i++) {
				for (int j = 0; j < c; j++) {
					printf("%c", ret[i][j]);
				}
				printf("\n");
			}
		}
	}

	return 0;
}
