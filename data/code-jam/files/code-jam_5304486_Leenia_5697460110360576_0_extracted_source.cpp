#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stack>
#include <list>

#define IMAX 1234567890

using namespace std;

int findl(int a, int b) {
	int r = (int)((float)a * 0.9) / b;
	while ((r * 11 * b) < (a * 10)) {
		r++;
	}
	if ((r) * 9 * b <= (a * 10)) return r;
	return -1;
}

int findr(int a, int b) {

	int r = a / b;
	while ((r * 9 * b) <= (a * 10)) {
		r++;
	}
	if ((r - 1) * 11 * b >= (a * 10)) return r - 1;
	return -1;
}

int main(int argc, const char * argv[]) {
	int test;
	int m[51];
	int pp[51][51];
	int npp[51][51][2];
	int index[51];
	cin >> test;
	for (int z = 1; z <= test; z++) {
		int result = 0;
		int n, p;
		cin >> n >> p;
		memset(index, 0, sizeof(index));
		for (int i = 0; i < n; i++) scanf("%d", &m[i]);
		for (int i = 0; i < n; i++) for (int j = 0; j < p; j++) scanf("%d", &pp[i][j]);
		for (int i = 0; i < n; i++) sort(pp[i], pp[i] + p);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < p; j++) {
				npp[i][j][0] = findl(pp[i][j], m[i]);
				npp[i][j][1] = findr(pp[i][j], m[i]);
			}
		}
		for (int i = 0; i < p; i++) {
			int flag = 0;
			int l = npp[0][i][0];
			int r = npp[0][i][1];
			if (r == -1) continue;
			for (int j = 1; j < n; j++) {
				int cnt = 0;
				while (index[j] < p && (npp[j][index[j]][1] < l || npp[j][index[j]][0] == -1)) index[j]++;
				if (index[j] < p && npp[j][index[j]][0] <= r) {
					index[j]++;
					continue;
				}
				else {
					flag = 1;
					break;
				}
			}
			if (flag == 0) result++;
		}
		printf("Case #%d: %d\n", z, result);
	}
	return 0;
}