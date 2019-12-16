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

struct data {
	uint64_t val, amt, ptr;
};

#define MAX_V 10000000

void run() {
	uint64_t e, ef, r, n, i, x, max_x, ptr, ptr_c;
	data *v;

	cin >> e >> r >> n >> ws;

	v = new data[n]();

	for (i = 0; i < n; i++) {
		cin >> v[i].val;
		v[i].amt = 0;
		v[i].ptr = 0;
	}

	//cout << endl << "max energy = " << e << endl;
	//cout << "recovery = " << r << endl;

	bool done = false;
	max_x = 0;
	while (!done) {
		x = 0;
		ef = e;
		for (uint64_t k = 0; k < n; ++k) {
			v[k].amt = v[k].ptr;
			if (v[k].amt > ef) {
				v[k].amt = 0;
				v[k].ptr = 0;
				if (k + 1 < n)
					v[k + 1].ptr++;
				else
					done = true;
			}
			if (ef - v[k].amt + r > e)
				v[k].amt = (ef - v[k].amt + r) - e;
			if (k + 1 == n)
				v[k].amt = ef;
			//cout << "Have " << ef << "/" << e << " energy: spending " << v[k].amt << " energy on " << v[k].val << endl;

			x  += v[k].val * v[k].amt;
			ef -= v[k].amt;
			ef += r;
			if (ef > e)
				ef = e;
		}
		if (x > max_x)
			max_x = x;
		v[0].ptr++;
		//cout << "max is " << max_x << endl;
	}

	cout << max_x;

	delete [] v;
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

