#include <iostream>

using namespace std;

int calc(int x, int y)
{
	char *px, *py;
	if (x > 0) {
		px = "WE";
	} else {
		px = "EW";
		x = -x;
	}
	if (y > 0) {
		py = "SN";
	} else {
		py = "NS";
		y = -y;
	}
	for (int i = 0; i < x; i++) {
		cout << px;
	}
	for (int i = 0; i < y; i++) {
		cout << py;
	}
}

int main (int argc, char const* argv[])
{
	int T;
	cin >> T;
	for (int i = 1; i <= T; i++) {
		int x, y;
		cin >> x >> y;
		cout << "Case #" << i << ": ";
		calc(x, y);
		cout << endl;
	}
	return 0;
}
