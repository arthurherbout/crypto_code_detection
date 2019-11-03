#include <iostream>

using namespace std;


int solve (int l, int r, int c) {
	if (l * c >= r)
		return 0;

	r = (r + l - 1) / l;
	l = 1;

	for (long long pw=1; ; pw*=c)
		if (pw >= r || pw * pw >= r)
			return 1 + max (solve (l, (int)pw, c), solve ((int)pw, r, c));
}

int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	for (int tt=1; tt<=ts; ++tt) {

		int l, r, c;
		cin >> l >> r >> c;

		printf ("Case #%d: %d\n", tt, solve (l, r, c));
	}


}

