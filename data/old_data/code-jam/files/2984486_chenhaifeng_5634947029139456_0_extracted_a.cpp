#include <iostream>
#include <stdio.h>

#include <fstream>
#include <stdint.h>
#include <algorithm>
#include <vector>

using namespace std;

int main() {
	freopen("A-small-attempt0.in", "r", stdin);
	freopen("A-small-attempt0.out", "w", stdout);
	int T, t = 0, N, L;
	scanf("%d", &T);
	char chr[200];
	vector<string> intial, final, mid;
	while (t < T) {
		t++;
		scanf("%d%d", &N, &L);
		printf("Case #%d: ", t);
		intial.clear();
		final.clear();
		for (int i = 0; i < N; i++) {
			scanf("%s", chr);
			intial.push_back(chr);
		}
		for (int i = 0; i < N; i++) {
			scanf("%s", chr);
			final.push_back(chr);
		}
		mid.resize(N);
		for (int p = 0; p < N; p++) {
			mid[p].resize(L);
		}
//		cout << endl;
//		cout << "______________________" << endl;
		sort(final.begin(), final.end());
//		for (int l = 0; l < N; l++) {
//			cout << final[l] << endl;
//		}
//		cout << "______________________" << endl;
		uint64_t size = 1 << L;
		uint64_t bit;
		int finish = -1;
		//cout << "size = " << size << endl;
		for (uint64_t i = 0; i < size; i++) {
			//cout << "i = " << i << " size = " << size << endl;
			if (finish != -1)
				break;

			uint64_t k = i;
			int cnt = 0;
			for (int j = 0; j < L; j++) {
				bit = k & 0x01;
				k >>= 1;
				//cout << "bit = "  << bit << endl;
				if (bit == 1) {
					cnt += 1;
					for (int p = 0; p < N; p++) {
						mid[p][j] = intial[p][j] == '1' ? '0' : '1';

					}
				} else {
					for (int p = 0; p < N; p++) {
						mid[p][j] = intial[p][j];
					}
				}

			}
//			for (int l = 0; l < N; l++) {
//				cout << intial[l] << endl;
//			}
			//cout << "_____________xxx_________" << endl;
			//for (int l = 0; l < N; l++) {
				//cout << mid[l] << endl;
			//}
			//cout << "XXXXXXXXXxxxxfasdfasdXXXXXXXXXXXXXXXXX" << endl;
			sort(mid.begin(), mid.end());
			int f;
			for (f = 0; f < N; f++) {
				if (mid[f] != final[f])
					break;
			}
			//cout << "f = " << f << "N = " << N << endl;
			//cout << "cnt = " << cnt << endl;
			if (f == N)
				finish = cnt;
			//for (int l = 0; l < N; l++) {
				//cout << mid[l] << endl;
			//}
			//cout << "XXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;

		}
		if (finish != -1) {
			printf("%d\n", finish);
		} else
			printf("NOT POSSIBLE\n");
	}

	return 0;
}
