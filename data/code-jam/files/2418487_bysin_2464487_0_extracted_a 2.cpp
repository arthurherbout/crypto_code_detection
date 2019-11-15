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

void run() {
	uint64_t r, t, c, f, g;

	cin >> r >> t >> ws;

	for (c = 0; ; c++) {
		g = r;
		r++;
		f = (r * r) - (g * g);
		if (t < f)
			break;
		t -= f;
		r++;
	}

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

