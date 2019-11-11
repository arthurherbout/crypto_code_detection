#include <iostream>
#include <string>
using namespace std;

int main() {

	int T;
	scanf("%d", &T);

	for (int tt = 1; tt <= T; tt++) {

		string cake;
		cin >> cake;

		int K;
		cin >> K;

		int cnt = 0;

		for (int i = 0; i <= cake.size() - K; i++) {
			if (cake[i] == '-') {
				for (int j = i; j < i + K; j++) {
					if (cake[j] == '-') cake[j] = '+';
					else if (cake[j] == '+') cake[j] = '-';
				}
				cnt++;
			}
		}
		bool chk = true;
		for (int i = 0; i < cake.size(); i++) {
			if (cake[i] == '-') chk = false;
		}
		printf("Case #%d: ", tt);
		if (chk) {
			printf("%d\n", cnt);
		}
		else {
			printf("IMPOSSIBLE\n");
		}
	}
}