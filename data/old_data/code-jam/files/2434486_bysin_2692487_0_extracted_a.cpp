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

uint64_t a, n, i, c;
vector<uint64_t> m;

uint64_t solve(vector<uint64_t>::iterator f, uint64_t a, uint64_t c, bool greed) {
	for (; f != m.end(); ) {
		uint64_t x = *f;

		//cout << "At mote " << x << " with energy " << a << endl;
		if (a > x) {
			//cout << " -- Absorbing" << endl;
			a += x;
			f++;
		}
		else {
			uint64_t c1 = 0xfffffff, c2;

			if (a != 1)
				c1 = solve(f, a + (a - 1), c + 1, true);
			c2 = solve(f + 1, a, c + 1, true);

			if (c1 < c2)
				return c1;
			return c2;

			/*if (a + (a - 1) <= x) {
				cout << " -- Removing" << endl;
				f++;
				c++;
			}
			else {
				cout << " -- Adding new energy " << (a - 1) << endl;
				a += (a - 1);
				c++;
			}*/
		}
	}
	return c;
}

void run() {
	cin >> a >> n;

	m.clear();
	for (i = 0; i < n; i++) {
		uint64_t x;
		cin >> x;
		m.push_back(x);
	}

	sort(m.begin(), m.end());

	c = solve(m.begin(), a, 0, false);

	cout << c;
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

