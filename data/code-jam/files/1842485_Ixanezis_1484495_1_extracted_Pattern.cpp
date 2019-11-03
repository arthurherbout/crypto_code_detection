#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <map>
#include <cstring>
#include <set>
#include <deque>
#include <cmath>
#include <stack>

using namespace std;

int n, w, l; 

bool byid = false;

struct student {
	long long rad;
	long long x, y;
	int id;

	bool operator < (const student& ot) const {
		if (byid)
			return id < ot.id;
		else
			return rad > ot.rad;
	}
};

student s[1111];

bool inside(const student& a, const student& b) {
	long long ab = a.x - a.rad;
	long long ae = a.x + a.rad;
	long long bb = b.x - b.rad;
	long long be = b.x + b.rad;

	if (ae <= bb) return false;
	if (ab >= be) return false;
	return true;
//	if (ab >= bb && ab <= be && ae >= bb && ae <= be) return true;
}

long long findBestY(const student& cur, long long curx, int number) {
	long long mx = 0;
	for (int i=0; i<number; i++) {
		if (inside(cur, s[i])) {
			mx = max(mx, s[i].y + s[i].rad + cur.rad);
		}
	}
	return mx;
}

int main() {
	//freopen("input.txt", "r", stdin);
	freopen("outputB.txt", "w", stdout);

	int t; cin >> t;
	
	for (int e=1; e<=t; e++) {
		cin >> n >> w >> l;
		byid = false;
		for (int i=0; i<n; i++) {
			cin >> s[i].rad;
			s[i].id = i;
		}

		sort(s, s+n);

		long long prvx = -s[0].rad;
		bool can = true;
		for (int i=0; i<n; i++) {
			long long curx = prvx + s[i].rad;
			if (curx <= w) {
				s[i].x = curx;
				s[i].y = findBestY(s[i], curx, i);
				if (s[i].y > l) {
					can = false;
					break;
				}
				prvx += s[i].rad*2;
			} else {
				prvx = -s[i].rad;
				--i;
			}
		}

		byid = true;
		sort(s, s+n);

		cout << "Case #" << e << ":";
		if (!can) {
			cout << " OOPS";
		} else {
			for (int i=0; i<n; i++) {
				//cout << fixed << setprecision(10) << ' ' << s[i].x << ' ' << s[i].y;
				cout << ' ' << s[i].x << ' ' << s[i].y;
			}
		}
		
		cout << endl;
	}

    return 0;
}