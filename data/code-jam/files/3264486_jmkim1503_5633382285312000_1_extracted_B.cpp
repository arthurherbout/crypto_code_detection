#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int main() {

	int T;
	scanf("%d", &T);

	for (int tt = 1; tt <= T; tt++) {

		string str;
		cin >> str;

		vector<int> num(str.length());

		for (int i = 0; i < num.size(); i++) num[i] = str[i] - '0';

		for (int i = num.size() - 1; i > 0; i--) {
			if (num[i - 1] > num[i]) {
				int j = i - 1;

				while (j>0 && num[j] == 0) {
					j--;
				}
				num[j]--;
				for (int k = j + 1; k < num.size(); k++) num[k] = 9;
			}
		}
		printf("Case #%d: ", tt);
		if (num[0] != 0) printf("%d", num[0]);
		for (int i = 1; i < num.size(); i++) {
			printf("%d", num[i]);
		}
		printf("\n");
	}
}