#include <iostream>
#include <string>
#include <cstring>
using namespace std;

string solve(string s) {
	string t;

	for (int i = 0; i < s.length(); i++) {
		string u, v;
		u += s[i]; u += t;
		v += t; v += s[i];
		if (strcmp(u.c_str(), v.c_str()) > 0)
			t = u;
		else
			t = v;
	}
	return t;
}

int main() {
	int t;
	cin >> t;
	for (int id = 1; id <= t; id++) {
		string s;
		cin >> s;
		cout << "Case #" << id << ": ";
		cout << solve(s) << endl;
	}
	return 0;
}