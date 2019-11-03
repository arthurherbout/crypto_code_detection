#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <cstring>

using namespace std;

int main() {

	int T;
	cin >> T;
	for (int cas = 1; cas <= T; cas++) {

		int odl_or[1001];
		int odlsort[1001];
		memset(odl_or, 0, sizeof(odl_or));
		memset(odlsort, 0, sizeof(odlsort));

		int L, t, N, C;
		cin >> L >> t >> N >> C;
		for (int i = 0; i < C; i++) {
			cin >> odl_or[i];
			odlsort[i] = odl_or[i];
		}
		sort(odlsort, odlsort + C, greater<int> ());

		int odl_count[1001];
		memset(odl_count, 0, sizeof(odl_count));

		int mod = (N - 1) % C;
		int div = (N - 1) / C;

		int odleglosc[1001];
		int gwiazdy[1001];
		for (int i = 0; i <= N; i++) {
			gwiazdy[i] = odl_or[i % C];
			if (i == 0) {
				odleglosc[i] = 0;
			} else {
				odleglosc[i] = odleglosc[i - 1] + gwiazdy[i - 1];
			}

		}

		//budujemy na najdłuższych dostępnych
		int akceleratory[1000];
		memset(akceleratory, 0, sizeof(akceleratory));
		if (L > 0) {
			for (int i = 0; i < N; i++) {
				if (odleglosc[i] * 2 + gwiazdy[i] * 2 >= t) {
					if (gwiazdy[i] == odlsort[0]) {
						akceleratory[i] = 1;
						L--;
					}
				}
				if (L == 0)
					break;
			}
		}

		if (L > 0 && C > 1) {
			for (int i = 0; i < N; i++) {
				if (odleglosc[i] + gwiazdy[i] >= t) {
					akceleratory[i] = 1;
					L--;
					break;
				}
			}
		}

		double czas = 0;
		for (int i = 0; i < N; i++) {
			if (akceleratory[i] == 0) {
				czas += gwiazdy[i] * 2;
			} else {
				if (odleglosc[i]*2 >= t ) {
					czas+=gwiazdy[i];
				} else {
					int czasZwykly  = t - odleglosc[i]*2;
					czas+= czasZwykly;
					double czasPrzysp = (double)(gwiazdy[i]*2 - czasZwykly)/(double)2;
					czas+=czasPrzysp;
				}
			}
		}

		cout << "Case #" << cas << ": " << (int)czas << endl;
	}

	return 0;
}
