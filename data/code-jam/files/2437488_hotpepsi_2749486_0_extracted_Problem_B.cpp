// Google Code Jam 2013 R1B
// Problem B. 

#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>

using namespace std;

typedef pair<int, int> II;
typedef map<II, char> IICMap;
typedef vector<II> IIVec;

string solve(int X, int Y)
{
	IICMap m;

	m[II(0,0)] = -1;

	IIVec q;
	q.push_back(II(0, 0));
	int d = 1;
	while (q.size() > 0) {
		IIVec next;
		int i;
		for (i = 0; i < (int)q.size(); ++i) {
			int x = q[i].first;
			int y = q[i].second;
			if (x == X && y == Y) {
				string ans;
				for (i = d-1; i >= 1; --i) {
					ans += m[II(x,y)];
					switch (m[II(x,y)]) {
					case 'W':
						x += i;
						break;
					case 'E':
						x -= i;
						break;
					case 'S':
						y += i;
						break;
					case 'N':
						y -= i;
						break;
					}
				}
				reverse(ans.begin(), ans.end());
				return ans;
			}
			int a, b;
			if (X != 0) {
				a = x-d, b = y;
				if (m.count(II(a,b)) == 0) {
					m[II(a,b)] = 'W';
					next.push_back(II(a, b));
				}
				a = x+d;
				if (m.count(II(a,b)) == 0) {
					m[II(a,b)] = 'E';
					next.push_back(II(a, b));
				}
			}
			if (Y != 0) {
				a = x, b = y-d;
				if (m.count(II(a,b)) == 0) {
					m[II(a,b)] = 'S';
					next.push_back(II(a, b));
				}
				b = y+d;
				if (m.count(II(a,b)) == 0) {
					m[II(a,b)] = 'N';
					next.push_back(II(a, b));
				}
			}
		}
		q = next;
		d += 1;
	}

	return "";
}

int main(int argc, char *argv[])
{
	string s;
	getline(cin, s);
	int T = atoi(s.c_str());
	if (T <= 0) return 0;

	for (int t = 0; t < T; ++t) {
		int X = 0, Y = 0;
		getline(cin, s);
		stringstream ss(s);
		ss >> X >> Y;
		cout << "Case #" << (t+1) << ": " << solve(X, Y) << endl;
	}

	return 0;
}

