#include <iostream>
#include <vector>

using namespace std;

long  long gcd(long long x, long long y) {
	if (y == 0) return x;
	return gcd(y, x%y);
}

int work() {
	long long x, y;
	char ch;
	cin >> x >> ch >> y;
	
	long long temp = gcd(x, y);
	x = x / temp;
	y = y / temp;
	if ((y & (y-1)) > 0) return -1; 
	if (x > y) return -1;
	
	int topx=0, topy=0;
	while (x > 0) {
		x = x >> 1;
		topx++;
	}
	while (y > 0) {
 		y = y >> 1;
		 topy++; 
	} 
	
	if (topy > 40) return -1;
	return topy-topx;
}

int main() {
	freopen("A-large.in", "r", stdin);
	freopen("A-large.out", "w", stdout);
	
	int c;
	cin >> c;
	for (int i=0; i<c; i++) {
		cout << "Case #" << i+1 << ": ";
		int ans = work();
		if (ans == -1) cout << "impossible" << endl;
		else cout << ans << endl;
	}
}