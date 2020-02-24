#include <stdio.h>

bool proc(int num) {

	int temp = num;

	int bef = temp % 10;

	temp /= 10;

	while (temp > 0) {
		int now = temp % 10;
		if (now > bef) return false;
		bef = now;
		temp /= 10;
	}
	return true;
}

int cal(int num) {
	int saver = 1;
	for (int i = 1; i <= num; i++) {
		if (proc(i)) saver = i;
	}
	return saver;
}

int main() {

	int T;
	scanf("%d", &T);

	for (int tt = 1; tt <= T; tt++) {
		int num;
		scanf("%d", &num);
		printf("Case #%d: %d\n", tt, cal(num));
	}
}