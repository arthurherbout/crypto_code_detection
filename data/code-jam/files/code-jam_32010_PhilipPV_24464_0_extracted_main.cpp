#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>

using namespace std;

#define X first
#define Y second

typedef pair<long long, long long> pll;

long long sb1, sb2, sc1, sc2, f1, f2;
bool ok;
double bb, cc;

void solve() {
	long long det = sb1 * sc2 - sc1 * sb2;
	if (!det) {
		if (sb1 + sc1 == f1 && sb2 + sc2 == f2) {
			bb = 1; cc = 1;
			ok = true;
		} else ok = false;
		return;
	}


	long long _b = f1 * sc2 - f2 * sc1;
	long long _c = sb1 * f2 - sb2 * f1;
	if (!_b && !_c) ok = false;
	else if (det > 0 && (_b < 0 || _c < 0)) ok = false;
	else if (det < 0 && (_b > 0 || _c > 0)) ok = false;
	else {
		bb = _b / (double)det;
		cc = _c / (double)det;
		ok = true;
	}
}

int main() {
//	ifstream cin("in");
	int _tc; cin >> _tc;
	cout.precision(10);
	for (int _t = 1; _t <= _tc; _t++) {
		pll a, b, c, am, bm, cm;
		cin >> a.X >> a.Y >> b.X >> b.Y >> c.X >> c.Y >> am.X >> am.Y >> bm.X >> bm.Y >> cm.X >> cm.Y;
		ok = false;
		
		double aa = 0;
		sb1 = b.X - bm.X; sb2 = b.Y - bm.Y;
		sc1 = c.X - cm.X; sc2 = c.Y - cm.Y;
		f1 = f2 = 0;
		solve();
		if (!ok) {
			aa = 1;
			f1 = am.X - a.X;
			f2 = am.Y - a.Y;
			solve();
		}

		cout << "Case #" << _t << ": ";
		if (a.X == am.X && a.Y == am.Y) cout << a.X << " " << a.Y;
		else if (b.X == bm.X && b.Y == bm.Y) cout << b.X << " " << b.Y;
		else if (c.X == cm.X && c.Y == cm.Y) cout << c.X << " " << c.Y;
		else if (!ok) cout << "No Solution";
		else 
			cout << ((aa * a.X + bb * b.X + cc * c.X) / (aa + bb + cc)) << " " << ((aa * a.Y + bb * b.Y + cc * c.Y) / (aa + bb + cc));
		cout << endl;
	}

	return 0;
}