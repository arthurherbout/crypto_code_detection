#include <iostream>
#include <string>
#include <string.h>
#include <map>
#include <vector>
using namespace std;


int bestturn = 2e9;

int hd, ad, hk, ak, b, d;
void go(int chd, int cad, int chk, int cak, int a, int c, int turn) {
	// cout << chd << " " << cad << " " << chk << " " << cak << " " << a << " " << c << " " << turn <<endl;
	if (turn > bestturn) {
		return;
	}
	if (chd <= 0) {
		return;
	}
	if (chk <= 0) {
		bestturn = turn;
		return;
	}
	if (b != 0 && cad <= chk && a == 0) {
		// cout << "b" << endl;
		go(chd-cak, cad+b, chk, cak, 0, 0, turn+1);
	}
	// cout << "a" << endl;
	go(chk-cad > 0 ? chd-cak: chd, cad, chk-cad, cak, 1, 0, turn+1);
	if (d != 0 && cak > 0 && a == 0) {
		// cout << "d" << endl;
		go(chd-cak+d, cad, chk, cak-d, 0, 0, turn+1);
	}
	if (chd <= cak && c == 0) {

		// cout << "c" << endl;
		go(hd-cak, cad, chk, cak, a, 1, turn+1);
	}

}
int main () {
	int t;
	cin >> t;
	for (int ti = 1; ti <= t; ti ++) {
		cin >> hd >> ad >> hk >> ak >> b >> d;
		bestturn = 2e9;
		go(hd, ad, hk, ak, 0, 0, 0);
		if (bestturn < 2e9) {
			cout << "Case #" << ti << ": "<< bestturn << endl;
		} else {
			cout << "Case #" << ti << ": IMPOSSIBLE"  << endl;
		}
		

	}
}