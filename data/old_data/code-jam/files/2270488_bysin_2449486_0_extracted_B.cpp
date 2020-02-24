/*
 * Written by Benjamin Kittridge
 *     for Google Code Jam
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <bitset>
#include <stack>
#include <iomanip>
#include <boost/algorithm/string.hpp>

using namespace std;

#define MAX_N 100
#define MAX_M 100

bool match(uint32_t n, uint32_t m, uint32_t hm[MAX_N][MAX_M], uint32_t cm[MAX_N][MAX_M]) {
	uint32_t i, j;
	bool bad;

	for (bad = false, i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			if (cm[i][j] != hm[i][j]) {
				bad = true;
				break;
			}
		}
		if (bad)
			break;
	}

	if (!bad) {
		cout << "YES";
		return true;
	}
	return false;
}

void run() {
	uint32_t n, m, i, j, max_c;
	uint32_t hm[MAX_N][MAX_M],
		 cm[MAX_N][MAX_M];

	cin >> n >> m;

	for (i = 0; i < n; i++)
		for (j = 0; j < m; j++) {
			cin >> hm[i][j];
			cm[i][j] = 100;
		}

	for (i = 0; i < n; i++) {
		max_c = 1;
		for (j = 0; j < m; j++) {
			if (hm[i][j] > max_c)
				max_c = hm[i][j];
		}
		for (j = 0; j < m; j++) {
			if (cm[i][j] > max_c)
				cm[i][j] = max_c;
		}
	}

	if (match(n, m, hm, cm))
		return;

	for (j = 0; j < m; j++) {
		max_c = 1;
		for (i = 0; i < n; i++) {
			if (hm[i][j] > max_c)
				max_c = hm[i][j];
		}
		for (i = 0; i < n; i++) {
			if (cm[i][j] > max_c)
				cm[i][j] = max_c;
		}
	}

	if (match(n, m, hm, cm))
		return;

	cout << "NO";
}

int main(int argc, char **argv) {
	uint32_t n;

	cin >> n >> ws;

	for (uint32_t c = 1; n--; c++) {
		cout << "Case #" << c << ": ";
		run();
		cout << endl;
	}
	return 0;
}

