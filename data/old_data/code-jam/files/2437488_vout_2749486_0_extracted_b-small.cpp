#include <iostream>
#include <string>

using namespace std;

int main() {
	int Tc;
	cin >> Tc;
	int x, y;
	for (int re = 1; re <= Tc; ++re) {
		cin >> x >> y;
		cout << "Case #" << re << ": ";

		if (x < 0) {
			for (int i = 0; i > x; --i) {
				cout << "EW";
			}
		} else {
			for (int i = 0; i < x; ++i) {
				cout << "WE";
			}
		}
		if (y < 0) {
			for (int i = 0; i > y; --i) {
				cout << "NS";
			}
		} else {
			for (int i = 0; i < y; ++i) {
				cout << "SN";
			}
		}
		cout << '\n';
	}
	return 0;
}
