#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main() {
	//FILE * fin = fopen("A-small-attempt0.in", "r");
	//FILE * fout = fopen("A-small-attempt0.out", "w");
	vector<vector<int> > a(4, vector<int>(4, 0)), b(4, vector<int>(4, 0));
	int t, s1, s2, m;
	scanf("%d", &t);
	for (int k = 0; k < t; k++) {
		scanf("%d", &s1);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				scanf("%d", &m);
				a[i][j] = m;
			}
		}

		scanf("%d", &s2);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				scanf("%d", &m);
				b[i][j] = m;
			}
		}
		s1--;
		s2--;
		printf("Case #%d: ", k + 1);
		int ans = 0;
		int finish = 0;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (finish == 1)
					break;
				if (a[s1][i] == b[s2][j]) {
					if (ans == 0)
						ans = a[s1][i];
					else {
						printf("Bad magician!\n");
						finish = 1;
					}
				}
			}
		}
		if (finish == 0) {
			if (ans != 0)
				printf("%d\n", ans);
			else
				printf("Volunteer cheated!\n");
		}
	}
	//fclose(fin);
	//fclose(fout);
	return 0;
}
