#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <iostream>
using namespace std;

ifstream fin("C-large.in");
string key = "welcome to code jam";

int main() {
	int tt; fin >> tt; string s; getline(fin, s);
	FILE * fout = fopen("output.txt", "w");
	for (int t = 0; t < tt; t++) {
		getline(fin, s);
		int len = s.length();

		vector<int> prev(key.length(), 0);
		if (s[0] == key[0])
			prev[0] = 1;
		for (int i = 1; i < len; i++) {
			vector<int> next(key.length(), 0);
			for (int j = 1; j < key.length(); j++)
				if (s[i] == key[j])
					next[j] = prev[j - 1];
			if (s[i] == key[0])
				next[0]++;
			for (int j = 0; j < key.length(); j++)
				next[j] = (next[j] + prev[j]) % 10000;
			prev = next;
		}
		fprintf(fout, "Case #%d: %04d\n", t + 1, prev[key.length() - 1]);
	}
	return 0;
}