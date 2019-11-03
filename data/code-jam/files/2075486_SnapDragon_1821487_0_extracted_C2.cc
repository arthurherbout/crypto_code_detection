#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Tile {
	long long x, y;
	char ch;
	bool operator<(const Tile& t) const {return x+y < t.x+t.y;}
};

int N;
vector<Tile> t;
inline long long LAbs(long long x) {return x < 0 ? -x : x;}

void Try(long long x, long long y, long long& retx, long long& rety) {
//cout << "  Trying " << x << ' ' << y << endl;
	long long d1 = LAbs(x) + LAbs(y);
	long long d2 = LAbs(retx) + LAbs(rety);
	if (d1 > d2) return;
	if (d1 < d2 || d1 == d2 && x > retx || d1 == d2 && x == retx && y > rety) {
		retx = x;
		rety = y;
//cout << "  ret is now " << retx << ' ' << rety << endl;
	}
}

main() {
	int T, prob = 1;
	for (cin >> T; T--;) {
		cin >> N;
		t.resize(N);
		for (int i = 0; i < N; i++) cin >> t[i].x >> t[i].y >> t[i].ch;
		long long retx = 1e18, rety = 1e18;
		for (int x = -1000; x <= 1000; x++)
		for (int y = -1000; y <= 1000; y++) {
			bool fail = false;
			for (int i = 0; i < N; i++) {
				int d = max(LAbs(t[i].x-x), LAbs(t[i].y-y));
				if ((t[i].ch == '#') ^ (d&1)) fail = true;
			}
			if (!fail) Try(x, y, retx, rety);
		}
		if (retx == 1e18) {
			cout << "Case #" << prob++ << ": Too damaged" << endl;
		} else {
			cout << "Case #" << prob++ << ": " << retx << ' ' << rety << endl;
		}
	}
}
