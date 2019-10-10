#include <string>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

ifstream fin("A-large.in");
ofstream fout("output.txt");
int len, d, n;

bool match(string s, string check) {
	int pos = 0;
	for (int i = 0; i < len; i++) {
		if (isalpha(s[pos])) {
			if (s[pos] != check[i])
				return 0;
			pos++;
		} else {
			bool ok = 0;
			while (s[pos] != ')') {
				if (s[pos] == check[i])
					ok = 1;
				pos++;
			}
			pos++;
			if (!ok)
				return 0;
		}
	}
	return 1;
}		

int main() {
	fin >> len >> d >> n;

	vector<string> v;
	for (int i = 0; i < d; i++) {
		string s;
		fin >> s;
		v.push_back(s);
	}

	vector<int> res(n, 0);	
	for (int i = 0; i < n; i++) {
		string s; fin >> s;
		for (int j = 0; j < d; j++)
			if (match(s, v[j]))
				res[i]++;
	}
	for (int i = 0; i < n; i++)
		fout << "Case #" << i + 1 << ": " << res[i] << endl;
	return 0;
}