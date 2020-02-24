#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void testcase() {
	int k,c,s;
	cin >> k >> c >> s;
	for (int i = 0; i < s; ++i) {
		long long x = i+1;
		for (int j = 1; j < c; ++j) {
			x += i*(long long)pow(k,j);
		}
		cout << x<< " ";
	}
}

int main() {
	int t;
	cin >> t;
	for (int i = 1; i <= t; ++i) {
		cout << "Case #" << i << ": ";
	    testcase();
		cout << endl;
	}
}
