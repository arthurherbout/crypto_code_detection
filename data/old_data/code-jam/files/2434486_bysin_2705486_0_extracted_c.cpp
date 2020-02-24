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

vector<char*> dict;

inline bool compare(char *a, char *b, uint32_t len, int32_t *index, uint32_t *change) {
	char *ap, *bp;
	uint32_t k = 0;

	for (ap = a, bp = b; ap < a + len; ) {
		if (*ap != *bp) {
			if (*index >= 0 && ap - a < (*index) + 5)
				return false;
			*index = ap - a;
			k++;
		}
		ap++, bp++;
	}
	*change = k;
	return true;
}

bool loop(char *ptr, int32_t *index, uint32_t *change) {
	//char *found;
	uint32_t len, large, fchange, mchange;
	int32_t findex, mindex;

	if (!*ptr)
		return true;
	//cout << "-- Lookup " << ptr << "\n";
	fchange = 0xffffff;
	for (auto f = dict.begin(); f != dict.end(); ++f) {
		char *d = *f;

		uint32_t mlen = strlen(d);
		mindex = *index;
		if (compare(ptr, d, mlen, &mindex, &mchange)) {
			if (mchange < fchange || (mchange == fchange && mlen > large)) {
				if (loop(ptr + mlen, &mindex, &mchange)) {
					//found = d;
					fchange = mchange;
					large = mlen;
				}
			}
		}
	}
	if (fchange == 0xffffff)
		return false;
	//cout << "-- Found word " << found << " " << fchange << "\n";
	*change += fchange;
	return true;
}

void run() {
	char str[1 << 10];
	uint32_t len, change;
	int32_t index;

	if (!fgets(str, sizeof(str), stdin))
		return;
	
	len = strlen(str);
	while (len && isspace(str[len - 1]))
		str[--len] = 0;

	//cout << "Initial word " << str << "\n";

	index = -1;
	change = 0;
	loop(str, &index, &change);

	cout << change;
}

void build() {
	FILE *f = fopen("garbled_email_dictionary.txt", "r");
	char buf[1 << 10];

	while (1) {
		if (!fgets(buf, sizeof(buf), f))
			break;
		uint32_t len = strlen(buf);
		while (len && isspace(buf[len - 1]))
			buf[--len] = 0;
		dict.push_back(strdup(buf));
	}
}

int main(int argc, char **argv) {
	uint32_t n;

	build();

	cin >> n >> ws;

	for (uint32_t c = 1; n--; c++) {
		cout << "Case #" << c << ": ";
		run();
		cout << endl;
	}
	return 0;
}

