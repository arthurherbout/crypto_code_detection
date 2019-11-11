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
#include <gmp.h>

using namespace std;

bool ispal(char *str) {
	char *ptr, *end;

	ptr = str;
	end = str + strlen(str) - 1;

	while (ptr < end) {
		if (*ptr != *end)
			return false;
		ptr++;
		end--;
	}
	return true;
}

void run() {
	char buf[1 << 10], *ptr;
	uint64_t num;

	if (!fgets(buf, sizeof(buf), stdin))
		return;

	for (ptr = buf; *ptr; ptr++)
		if (*ptr == '\n' || *ptr == '\r')
			*ptr = 0;

	for (ptr = buf; *ptr && *ptr != ' '; ptr++);
	if (!*ptr)
		return;
	*ptr++ = 0;

	mpz_t a, b, c;

	mpz_init(a);
	mpz_init(b);
	mpz_init(c);

	mpz_set_str(a, buf, 10);
	mpz_set_str(b, ptr, 10);

	num = 0;
	while (mpz_cmp(a, b) <= 0) {
		if (mpz_root(c, a, 2)) {
			mpz_get_str(buf, 10, a);
			if (ispal(buf)) {
				mpz_get_str(buf, 10, c);
				if (ispal(buf))
					num++;
			}
		}

		mpz_add_ui(a, a, 1);
	}

	cout << num;
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

