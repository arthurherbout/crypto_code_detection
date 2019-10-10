// Google Code Jam 2015 R2
// Problem D.

#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>

using namespace std;

typedef long long LL;

LL solve(LL R, LL C) {
	int ans[7][7] = {
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 2, 1, 1, 2 },
		{ 0, 0, 0, 2, 2, 2, 2 },
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 0, 0, 0, 1, 1, 1, 1 },
		{ 0, 0, 0, 2, 2, 2, 2 }
	};
	return ans[R][C];
}

int main(int argc, char *argv[]) {
	string s;
	getline(cin, s);
	LL T = atoi(s.c_str());
	for (LL t = 1; t <= T; ++t) {
		LL R, C;
		getline(cin, s);
		stringstream ss(s);
		ss >> R >> C;
		cout << "Case #" << t << ": " << solve(R, C) << endl;
	}
	return 0;
}
